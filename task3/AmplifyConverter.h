#pragma once

#include "Converter.h"

#include <vector>
#include <cstdint>
#include <algorithm> // Для std::clamp

class AmplifyConverter : public Converter {
private:
    double amplification_factor; 
    double start_time;
    double end_time;
    int sample_rate;
public:
    AmplifyConverter(double amplification_factor, double start_time, double end_time, int sample_rate); // параметр - коэффициент усиления амплитуды

    std::vector<int16_t> Convert(const std::vector<int16_t>& input) override;
};
