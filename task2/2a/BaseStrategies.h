// 1
#pragma once

#include "Strategy.h"

class PoorTrustingFool : public Strategy {
public: 
    PoorTrustingFool() {}
    char MakeMove(const std::vector<char>& my_history, const std::vector<char>& opp1_history, const std::vector<char>& opp2_history) override {
        return 'C';
    }
    void reset() override {}
    // override is here for test purposes
    // https://ravesli.com/urok-164-modifikatory-override-i-final-kovariantnyj-tip-vozvrata/#toc-0
};

// 2
class AllDefect : public Strategy {
public: 
    AllDefect() {}
    char MakeMove(const std::vector<char>& my_history, const std::vector<char>& opp1_history, const std::vector<char>& opp2_history) override {
        return 'D';
    }
    void reset() override {}
};