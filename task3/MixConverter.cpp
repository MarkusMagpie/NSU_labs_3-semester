#include "MixConverter.h"

#include <algorithm>
#include <cmath>

#include <iostream>

//Task - take main stream and second sctream, start mixing starting from start_time
MixConverter::MixConverter(const std::vector<int16_t>& second_stream, double start_time, double end_time, double sample_rate)
    : second_stream(second_stream), start_time(start_time), end_time(end_time), sample_rate(sample_rate) {}
    
std::vector<int16_t> MixConverter::Convert(const std::vector<int16_t>& input) {
    auto starting_sample = static_cast<size_t>(start_time * sample_rate);
    auto ending_sample = static_cast<size_t>(end_time * sample_rate);

    std::vector<int16_t> output (input); // copy of input where we will mix samples

    // length of both streams (1 - main stream (input), 2 - second stream (second_stream))
    auto main_stream_len = input.size();
    auto second_stream_len = second_stream.size();

    std::cout << "\nSTART MIXING" << std::endl;

    std::cout << "Main sample string length: " << main_stream_len << std::endl;
    std::cout << "Second sample string length: " << second_stream_len << "\n" << std::endl;

    std::cout << "Mix samples from " << start_time << " to " << end_time << " seconds" << std::endl;
    std::cout << "Mix samples from " << starting_sample << " to " << ending_sample << " samples" << std::endl;

    if (starting_sample >= main_stream_len) return output;
    ending_sample = std::min(ending_sample, main_stream_len);

    for (size_t i = starting_sample; i < ending_sample; ++i) {
        // size_t second_stream_index = i - starting_sample;

        // if (i >= second_stream_len) {
        //     throw std::runtime_error("Second stream is shorter than the main stream, stopping mix.");
        // }

        // size_t average_sample = (output[i] + second_stream[i]) / 2;
        // output[i] = static_cast<int16_t>(average_sample);

        if (i - starting_sample >= second_stream_len) {
            output[i] = 0;
        } else {
            int average_sample = static_cast<int>(output[i]) + static_cast<int>(second_stream[i - starting_sample]);
            output[i] = static_cast<int16_t>(average_sample / 2);
        }
    }

    std::cout << "END MIXING" << std::endl;

    return output;
}