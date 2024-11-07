#pragma once

#include <cstdint>

// https://docs.fileformat.com/audio/wav/

struct WAVHeader {
    // riff chunk
    char riffHeader[4];        // "RIFF"
    uint32_t wavSize;          // Размер всего файла - 8 байт = 44 - 8 = 36 + размер аудиоданных
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
};

// 3 data chunk - в структуре поскольку между fmt и data чанками может быть еще что-о и нужно пропустить 
struct WAVChunkBase {
    char chunkid[4];
    int chunksize = 0;
};
