#include "MixConverter.h"

#include <algorithm>
#include <cmath>

//Task - take main stream and second sctream, start mixing starting from insert_time
MixConverter::MixConverter(const std::vector<int16_t>& second_stream, double insert_time, int sample_rate)
    : second_stream(second_stream), insert_time(insert_time), sample_rate(sample_rate) {}
    

std::vector<int16_t> MixConverter::Convert(const std::vector<int16_t>& input) {
    // count starting sample index for insertion
    size_t insert_sample_index = static_cast<size_t>(insert_time * sample_rate);

    std::vector<int16_t> output = input; // copy of input where we will mix samples

    // length of both streams (1 - main stream (input), 2 - second stream (second_stream))
    size_t main_stream_len = input.size();
    size_t second_stream_len = second_stream.size();

    if (insert_sample_index >= main_stream_len) return output; // case if nothing to mix

    // iterating over samples from insertion point to the end of the main stream
    // ! If second stream is shorter than main stream, then for indexes bigger then length of second stream, we will get Segmentation fault

    for (size_t i = insert_sample_index; i < main_stream_len; ++i) {
        if (i - insert_sample_index < second_stream_len) {
            size_t average_value = (output[i] + second_stream[i - insert_sample_index]) / 2;
            output[i] = static_cast<int16_t>(average_value);
        }
    }

    return output;
}
