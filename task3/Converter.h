#pragma once

#include <vector>
#include <cstdint>

class Converter {
public:
    // Converter() = default;
    virtual ~Converter() {};
    virtual std::vector<int16_t> Convert(const std::vector<int16_t>& input) = 0;
};