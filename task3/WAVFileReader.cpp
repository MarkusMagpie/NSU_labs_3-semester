#include "WAVFileReader.h"

#include <iostream>

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

    if (header.audioFormat != 1 ||
        header.numChannels != 1 ||
        header.bitsPerSample != 16) {
        throw std::runtime_error("Wrong WAV file format, must be PCM, 16-bit mono audio");
    }

    sample_rate = header.sample_rate;
    num_samples = header.dataSize / (header.bitsPerSample / (8 * header.numChannels)) * sample_rate; // всего байтов / байтов на 1 сэмпл * частота
}

std::vector<int16_t> WAVFileReader::ReadSamples() {
    std::vector<int16_t> samples(num_samples);
    // samples.data() - указатель на первый элемент вектора samples
    // reinterpret_cast<char*>(samples.data()) - преобразование int16_t* указателя на первый элемент вектора samples в указатель char*
    // num_samples * sizeof(int16_t) - общее количество байт всех сэмплов в векторе
    input.read(reinterpret_cast<char*>(samples.data()), num_samples * sizeof(int16_t));

    std::cout << "Amount of read samples: " << num_samples << std::endl;
    
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