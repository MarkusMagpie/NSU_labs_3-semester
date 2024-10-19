#include "MixConverter.h"

#include <algorithm>
#include <cmath>

#include <iostream>

//Task - take main stream and second sctream, start mixing starting from start_time
MixConverter::MixConverter(const std::vector<int16_t>& second_stream, double start_time, double end_time, int sample_rate)
    : second_stream(second_stream), start_time(start_time), end_time(end_time), sample_rate(sample_rate) {}
    
std::vector<int16_t> MixConverter::Convert(const std::vector<int16_t>& input) {
    // count starting sample index for insertion
    size_t starting_sample = static_cast<size_t>(start_time * sample_rate);
    size_t ending_sample = static_cast<size_t>(end_time * sample_rate);

    std::vector<int16_t> output = input; // copy of input where we will mix samples

    // length of both streams (1 - main stream (input), 2 - second stream (second_stream))
    size_t main_stream_len = input.size();
    size_t second_stream_len = second_stream.size();

    std::cout << "\nSTART MIXING" << std::endl;

    if (starting_sample >= main_stream_len) {
        std::cout << "Starting point is after the end of the main (input) stream!" << std::endl;
        return output;
    }

    if (starting_sample >= second_stream_len) {
        std::cout << "Starting point is after the end of the second stream!" << std::endl;
        return output;
    }

    std::cout << "Main sample string length: " << main_stream_len << std::endl;
    std::cout << "Second sample string length: " << second_stream_len << "\n" << std::endl;

    std::cout << "Mix samples starting from " << start_time << " to " << end_time << " seconds" << std::endl;
    std::cout << "Mix samples between " << starting_sample << " and " << ending_sample << " samples" << "\n" << std::endl;

    // iterating over samples from insertion point to the end of the main stream
    // ! If second stream is shorter than main stream, then for indexes bigger then length of second stream, we will get Segmentation fault  

    ending_sample = std::min(ending_sample, main_stream_len);
    std::cout << "Mixing between sample indices: " << starting_sample << " to " << ending_sample << std::endl;

    for (size_t i = starting_sample; i < ending_sample; ++i) {
        // size_t second_stream_index = i - starting_sample;

        if (i >= second_stream_len) {
            throw std::runtime_error("Second stream is shorter than the main stream, stopping mix.");
        }

        if (second_stream[i] == 0) {
            std::cout << "Second stream sample is 0 at index: " << i << "; \n" << "starting sample: " << i << std::endl;
            return output;
        }

        size_t average_sample = (output[i] + second_stream[i]) / 2;
        output[i] = static_cast<int16_t>(average_sample);
    }

    std::cout << "END MIXING" << std::endl;

    return output;
}
