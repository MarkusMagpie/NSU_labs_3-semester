#pragma once

#include "Converter.h"
#include <algorithm>
#include <cstddef>

class MuteConverter : public Converter {
public:
    MuteConverter(double start_time, double end_time, int sample_rate);
    std::vector<int16_t> Convert(const std::vector<int16_t>& input) override;
private:
    double start_time; // start of the mute (seconds)
    double end_time; // end of the mute (seconds)
    int sample_rate; // number of samples per second
};