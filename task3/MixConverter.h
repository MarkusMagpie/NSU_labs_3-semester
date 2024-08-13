#pragma once

#include "Converter.h"

class MixConverter : public Converter {
public:
    // second_stream - дополнительный поток сэмплов, будет смешан с основным
    // insert_time - время (в секундах), с которого начнется смешивание
    // sample_rate - частота дискретизации (samples per second) - для индекса сэмпла
    MixConverter(const std::vector<int16_t>& second_stream, double insert_time, int sample_rate);

    // Метод convert смешивает основной поток с дополнительным
    std::vector<int16_t> Convert(const std::vector<int16_t>& input) override;
private:
    const std::vector<int16_t>& second_stream; // Ссылка на дополнительный поток
    double insert_time;   // Время начала смешивания в секундах
    int sample_rate;      // Частота дискретизации
};
