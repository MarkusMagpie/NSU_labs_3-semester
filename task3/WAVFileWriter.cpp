#include "WAVFileWriter.h"

WAVFileWriter::WAVFileWriter(std::string& filename, int sample_rate, int num_samples) : 
    output(filename, std::ios::binary) {
    if (!output.is_open()) {
        throw std::runtime_error("Failed to open WAV file for writing: " + filename);
    }
    
    this->sample_rate = sample_rate;
    this->num_samples = num_samples;

    WriteHeader();
}

void WAVFileWriter::WriteHeader() {
    WAVHeader header;
    
    std::memcpy(header.riffHeader, "RIFF", 4);
    std::memcpy(header.waveHeader, "WAVE", 4);
    std::memcpy(header.fmtHeader, "fmt ", 4);
    std::memcpy(header.dataHeader, "data", 4);
    
    // 36 байт - до начала данных - riffHeader + wavSize = 4 + 4 = 8. (44 - 8 = 36)
    header.wavSize = 36 + num_samples * sizeof(int16_t); // 36 байт до начала данных + размер данных
    header.fmtChunkSize = 16;  // Размер fmt-чанка
    header.audioFormat = 1;    // PCM формат
    header.numChannels = 1;    // Моно звук
    header.sample_rate = sample_rate;
    header.bitsPerSample = 16; // 16 бит на сэмпл
    header.byteRate = header.sample_rate * header.bitsPerSample * header.numChannels / 8;
    header.blockAlign = header.numChannels * header.bitsPerSample / 8;
    header.dataSize = num_samples * sizeof(int16_t);  // Размер аудиоданных = число сэмплов * 16 бит на сэмпл
    
    // Запись заголовка в файл
    output.write(reinterpret_cast<char*>(&header), sizeof(WAVHeader));
    if (!output) {
        throw std::runtime_error("Failed to write WAV header to file.");
    }
}

void WAVFileWriter::WriteSamples(std::vector<int16_t>& samples) {
    // if (samples.size() != static_cast<size_t>(num_samples)) {
    //     throw std::runtime_error("Number of samples does not match the expected count.");
    // }
    
    output.write(reinterpret_cast<char*>(samples.data()), samples.size() * sizeof(int16_t));
    
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