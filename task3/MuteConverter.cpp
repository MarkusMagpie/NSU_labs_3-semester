#include "MuteConverter.h"

MuteConverter::MuteConverter(double start_time, double end_time, int sample_rate) :
    start_time(start_time), end_time(end_time), sample_rate(sample_rate) {}

// converts all samples between start_time and end_time to 0
std::vector<int16_t> MuteConverter::Convert(const std::vector<int16_t>& input) {
    // std::vector<int16_t> output = input;
    // for (int i = start_time; i < end_time && i < output.size(); ++i) {
    //     output[i] = 0;
    // }
    // return output;

    auto start = static_cast<size_t>(start_time * sample_rate);
    auto end = static_cast<size_t>(end_time * sample_rate);

    std::vector<int16_t> output (input);

    if (start >= output.size()) return output;
    if (end > output.size()) end = output.size(); // to avoid index out of range

    for (size_t i = start; i < end; ++i) {
        output[i] = 0;
    }
    
    return output;
    
}
