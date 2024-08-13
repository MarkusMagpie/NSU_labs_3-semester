#pragma once

#include <fstream>
#include <string>
#include <vector>

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

class WAVFileReader {
private:
    std::ifstream input;
    int sample_rate;
    int num_samples;
public:
    WAVFileReader(const std::string& filename);
    ~WAVFileReader();
    
    void ReadHeader();
    std::vector<int16_t> ReadSamples();
    int GetSampleRate();
    int GetNumSamples();
};