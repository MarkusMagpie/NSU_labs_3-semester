#pragma once

#include <vector>
#include <cstdint>

class Converter {
public:
    virtual std::vector<int16_t> Convert(const std::vector<int16_t>& input);
    virtual ~Converter() = default;
};