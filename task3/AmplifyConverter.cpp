#include "AmplifyConverter.h"

#include <iostream>
#include <algorithm> // Для std::clamp

AmplifyConverter::AmplifyConverter(double amplification_factor, double start_time, double end_time, int sample_rate) :
    amplification_factor(amplification_factor), start_time(start_time), end_time(end_time), sample_rate(sample_rate) {}

std::vector<int16_t> AmplifyConverter::Convert(const std::vector<int16_t>& input) {
    std::cout << "\nSTART AMPLIFY" << std::endl;
    std::cout << "Amplify all samples by factor " << amplification_factor << " between " << start_time << " and " << end_time << std::endl;

    std::vector<int16_t> output(input);
    auto start = static_cast<size_t>(start_time * sample_rate);
    auto end = static_cast<size_t>(end_time * sample_rate);

    if (start >= output.size()) return output;
    if (end > output.size()) end = output.size(); // to avoid index out of range

    for (double i = start; i < end; i += 1.0) {
        int32_t amplified_sample = static_cast<int32_t>(output[i] * amplification_factor);
        output[i] = static_cast<int16_t>(std::clamp(amplified_sample, static_cast<int32_t>(INT16_MIN), static_cast<int32_t>(INT16_MAX)));
        // std::cout << "Amplified sample " << i << ": " << output[i] << "\n";
    }

    std::cout << "END AMPLIFY" << std::endl;
    
    return output;
}
