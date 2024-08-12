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

    // 1 count starting and ending sample indexes for mute
    auto start = static_cast<size_t>(start_time * sample_rate);
    auto end = static_cast<size_t>(end_time * sample_rate);

    // 2 create output vector
    std::vector<int16_t> output (input);
    
    // 2.1 check bounds of start and end indexes
    start = std::min(start, output.size());
    end = std::min(end, output.size());
    
    // 3 set mute
    std::fill(output.begin() + start, output.begin() + end, 0);
    return output;
    
}
