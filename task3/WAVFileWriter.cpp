#include "WAVFileWriter.h"

#include <iostream>

WAVFileWriter::WAVFileWriter(std::string& filename, int sample_rate, int num_samples, int wav_size, int data_size) : 
    output(filename, std::ios::binary) 
    {
        if (!output.is_open()) {
            throw std::runtime_error("Failed to open WAV file for writing: " + filename);
        }
        
        this->sample_rate = sample_rate;
        this->num_samples = num_samples;
        this->wav_size = wav_size;
        this->data_size = data_size;

        WriteHeader();
}

void WAVFileWriter::WriteHeader() {
    WAVHeader header;
    
    // заполняем заголовки текстовыми данными
    std::memcpy(header.riffHeader, "RIFF", 4);
    std::memcpy(header.waveHeader, "WAVE", 4);
    std::memcpy(header.fmtHeader, "fmt ", 4);
    std::memcpy(header.dataHeader, "data", 4);
    
    header.fmtChunkSize = 16;  // Размер fmt-чанка
    header.audioFormat = 1;    // PCM формат
    header.numChannels = 1;    // Моно звук
    header.sample_rate = sample_rate; // Частота сэмплов в секунду
    header.bitsPerSample = 16; // 16 бит на сэмпл
    header.byteRate = header.sample_rate * header.bitsPerSample * header.numChannels / 8;
    header.blockAlign = header.numChannels * header.bitsPerSample / 8;
    header.dataSize = data_size * sample_rate;
    header.wavSize = wav_size;

    std::cout << "\nRIFF chunk: " << std::endl;
    std::cout << "Riff header: " << header.riffHeader << std::endl;
    std::cout << "WAV size: " << header.wavSize << std::endl;
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
    std::cout << "Data size: " << header.dataSize << std::endl;
    std::cout << "Data header: " << header.dataHeader << std::endl;
    std::cout << "---" << std::endl;

    // Запись заголовка в файл
    output.write(reinterpret_cast<char*>(&header), sizeof(WAVHeader));

    if (!output) {
        throw std::runtime_error("Failed to write WAV header to file.");
    }
}

void WAVFileWriter::WriteSamples(std::vector<int16_t>& samples) {
    if (samples.size() != static_cast<size_t>(num_samples)) {
        throw std::runtime_error("Number of samples does not match the expected count");
    }

    std::cout << "\nWriting " << num_samples * sizeof(int16_t) << " samples to WAV file..." << std::endl;

    output.write(reinterpret_cast<char*>(samples.data()), num_samples * sizeof(int16_t));
    
    if (!output) {
        throw std::runtime_error("Failed to write WAV samples to file.");
    }
}

// destructor closes the file
WAVFileWriter::~WAVFileWriter() {
    if (output.is_open()) {
        output.close();
    }
}