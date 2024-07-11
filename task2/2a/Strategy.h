#pragma once

#include <vector>

class Strategy {
public:
    virtual ~Strategy() = default;
    
    virtual char MakeMove(const std::vector<char>& my_history, const std::vector<char>& opp1_history, const std::vector<char>& opp2_history) = 0;
};

// functions here are 'pure virtual' => any child class must provide implementations for these two functions
// A class that contains at least one pure virtual function (i have 1) is considered an abstract class.
//      Another child class from it mustprovide implementations for both pure virtual functions