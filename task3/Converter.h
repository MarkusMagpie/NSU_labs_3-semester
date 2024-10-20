#pragma once

#include <vector>
#include <cstdint>

class Converter {
public:
    virtual ~Converter() {};
    virtual std::vector<int16_t> Convert(const std::vector<int16_t>& input) = 0;
};