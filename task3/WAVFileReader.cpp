#include "WAVFileReader.h"

#include <iostream>

// заметка: If the file can be created in a regular path, the exception won’t be thrown, and the test will fail 
WAVFileReader::WAVFileReader(const std::string& filename) :
    input(filename, std::ios::binary) 
    {
        if (!input.is_open()) {
            throw std::runtime_error("Failed to open WAV file for reading: " + filename);
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
        throw std::runtime_error("Wrong WAV file format: must be PCM, 16-bit and mono audio");
    }

    WAVChunkBase chunk;
    
    do
    {
        input.ignore(chunk.chunksize);
        input.read(reinterpret_cast<char*>(&chunk), sizeof(WAVChunkBase));
        if (!input) {
            throw std::runtime_error("Failed to read WAV chunk from file");
        }
    } while (chunk.chunkid[0] != 'd' || chunk.chunkid[1] != 'a' || chunk.chunkid[2] != 't' || chunk.chunkid[3] != 'a');

    sample_rate = header.sample_rate;
    num_samples = chunk.chunksize / (header.numChannels * header.bitsPerSample / 8);

    std::cout << "\nRIFF chunk: " << std::endl;
    std::cout << "Riff header: " << header.riffHeader << std::endl;
    std::cout << "WAV size: " << header.wavSize << std::endl;
    wav_size = header.wavSize;
    std::cout << "Wave header: " << header.waveHeader << std::endl;
    std::cout << "---" << std::endl;

    std::cout << "\nfmt chunk: " << std::endl;
    std::cout << "FMT header: " << header.fmtHeader << std::endl;
    std::cout << "FMT chunk size: " << header.fmtChunkSize << std::endl;
    std::cout << "Audio format: " << header.audioFormat << std::endl;
    std::cout << "Number of channels: " << header.numChannels << std::endl;
    std::cout << "Sample rate: " << header.sample_rate << std::endl;
    std::cout << "Byte rate: " << header.byteRate << std::endl;
    std::cout << "Block align: " << header.blockAlign << std::endl;
    std::cout << "Bits per sample: " << header.bitsPerSample << std::endl;
    std::cout << "---" << std::endl;

    std::cout << "\ndata chunk: " << std::endl;
    std::cout << "Data size: " << chunk.chunksize << std::endl;
    data_size = chunk.chunksize;
    std::cout << "Data header: " << std::string(chunk.chunkid, 4) << std::endl;
    std::cout << "---" << std::endl;
}

std::vector<int16_t> WAVFileReader::ReadSamples() {
    std::vector<int16_t> samples(num_samples);
    // samples.data() - указатель на первый элемент вектора samples
    // reinterpret_cast<char*>(samples.data()) - преобразование int16_t* указателя на первый элемент вектора samples в указатель char*
    // num_samples * sizeof(int16_t) - общее количество всех сэмплов в векторе

    input.read(reinterpret_cast<char*>(samples.data()), num_samples * sizeof(int16_t)); // 1 вариант
    // 2 вариант
    // int16_t sample;
    // while (input.read(reinterpret_cast<char*>(&sample), (sizeof(int16_t)))) {
    //     samples.push_back(sample);
    // }

    return samples;
}

//getters
int WAVFileReader::GetSampleRate() {
    return sample_rate;
}

int WAVFileReader::GetNumSamples() {
    return num_samples;
}

int WAVFileReader::GetWavSize() {
    return wav_size;
}

int WAVFileReader::GetDataSize() {
    return data_size;
}

WAVFileReader::~WAVFileReader() {
    if (input.is_open()) {
        input.close();
    }
}