#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <cstring>

// https://docs.fileformat.com/audio/wav/

struct WAVHeader {
    // riff chunk
    char riffHeader[4];        // "RIFF"
    uint32_t wavSize;          // Размер файла - 8 байт = 44 - 8 = 36 + размер аудиоданных
    char waveHeader[4];        // "WAVE"
    
    // fmt chunk
    char fmtHeader[4];         // "fmt "
    uint32_t fmtChunkSize;     // Размер fmt чанка
    uint16_t audioFormat;      // Формат аудио (1 для PCM) (ВАЖНО)
    uint16_t numChannels;      // Количество каналов (1 - mono, 2 - stereo) (ВАЖНО)
    uint32_t sample_rate;      // Частота дискретизации
    uint32_t byteRate;         // Количество байт в секунду
    uint16_t blockAlign;       // Количество байт на сэмпл
    uint16_t bitsPerSample;    // Количество бит на сэмпл (ВАЖНО) 
    
    // data chunk
    char dataHeader[4];        // "data"
    uint32_t dataSize;         // Размер аудиоданных в байтах
};

class WAVFileWriter {
private:
    std::ofstream output;
    int sample_rate;
    int num_samples;
public:
    WAVFileWriter(std::string& filename, int sample_rate, int num_samples);
    ~WAVFileWriter();
    
    void WriteHeader();
    void WriteSamples(std::vector<int16_t>& samples);
};
