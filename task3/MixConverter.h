#pragma once

#include "Converter.h"

class MixConverter : public Converter {
public:
    // second_stream - дополнительный поток сэмплов, будет смешан с основным
    // start_time - время (в секундах), с которого начнется смешивание
    // end_time - время (в секундах), до которого будет смешиваться основной поток
    // sample_rate - частота дискретизации (samples per second) - для индекса сэмпла
    MixConverter(const std::vector<int16_t>& second_stream, double start_time, double end_time, int sample_rate);

    // Метод convert смешивает основной поток с дополнительным
    std::vector<int16_t> Convert(const std::vector<int16_t>& input) override;
private:
    const std::vector<int16_t>& second_stream;
    double start_time;
    double end_time;
    int sample_rate;
};
