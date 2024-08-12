#pragma once

#include "WAVFileReader.h"

// https://docs.fileformat.com/audio/wav/

struct WAVHeader {
    char riffHeader[4];        // "RIFF"
    uint32_t wavSize;          // Размер файла 8 байт
    char waveHeader[4];        // "WAVE"
    char fmtHeader[4];         // "fmt "
    uint32_t fmtChunkSize;     // Размер оставшейся части заголовка?
    uint16_t audioFormat;      // Формат аудио (1 для PCM) (ВАЖНО)
    uint16_t numChannels;      // Количество каналов (1 - mono, 2 - stereo) (ВАЖНО)
    uint32_t sampleRate;       // Частота дискретизации
    uint32_t byteRate;         // Количество байт в секунду
    uint16_t blockAlign;       // Количество байт на сэмпл
    uint16_t bitsPerSample;    // Количество бит на сэмпл (ВАЖНО) 
    char dataHeader[4];        // "data"
    uint32_t dataSize;         // Размер аудиоданных в байтах
};

WAVFileReader::WAVFileReader(const std::string& filename) :
    input(filename, std::ios::binary) 
    {
        if (!input.is_open()) {
            throw std::runtime_error("FAILED TO OPEN WAV FILE for reading: " + filename);
        }

        ReadHeader();
    }

// WAV-файл представляет собой контейнер для хранения звукового потока. 
// Он состоит из заголовка и данных в виде последовательности сэмплов.

void WAVFileReader::ReadHeader() {
    WAVHeader header;
    
    // read header
    input.read(reinterpret_cast<char*>(&header), sizeof(WAVHeader));
    if (!input) {
        throw std::runtime_error("Failed to read WAV header from file");
    }

    // check header
    if (std::string(header.riffHeader, 4) != "RIFF" ||
        std::string(header.waveHeader, 4) != "WAVE" ||
        std::string(header.fmtHeader, 4) != "fmt " ||
        std::string(header.dataHeader, 4) != "data") {
        throw std::runtime_error("Invalid WAV file format");
    }

    if (header.audioFormat != 1 ||
        header.numChannels != 1 ||
        header.bitsPerSample != 16) {
        throw std::runtime_error("Wrong WAV file format, must be PCM, 16-bit mono audio");
    }

    sample_rate = header.sampleRate;
    num_samples = header.dataSize / (header.bitsPerSample / 8); // всего байтов / байтов на 1 сэмпл
}

std::vector<int16_t> WAVFileReader::ReadSamples() {
    std::vector<int16_t> samples(num_samples);
    // samples.data() - указатель на первый элемент вектора samples
    // reinterpret_cast<char*>(samples.data()) - преобразование int16_t* указателя на первый элемент вектора samples в указатель char*
    // num_samples * sizeof(int16_t) - общее количество байт всех сэмплов в векторе
    input.read(reinterpret_cast<char*>(samples.data()), num_samples * sizeof(int16_t));
    
    if (!input) {
        throw std::runtime_error("Failed to read WAV samples from file");
    }

    return samples;
}

//getters
int WAVFileReader::GetSampleRate() {
    return sample_rate;
}

int WAVFileReader::GetNumSamples() {
    return num_samples;
}

WAVFileReader::~WAVFileReader() {
    if (input.is_open()) {
        input.close();
    }
}