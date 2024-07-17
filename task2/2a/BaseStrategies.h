// 1
#pragma once

#include "Strategy.h"

#include <cstdlib>
#include <ctime>

class PoorTrustingFool : public Strategy {
public: 
    PoorTrustingFool() {
        name = "PoorTrustingFool";
        description = "Always cooperates";
    }

    char MakeMove(const std::vector<char>& my_history, const std::vector<char>& opp1_history, const std::vector<char>& opp2_history) override {
        return 'C';
    }

    // override is here for test purposes
    // https://ravesli.com/urok-164-modifikatory-override-i-final-kovariantnyj-tip-vozvrata/#toc-0
};

// 2
class AllDefect : public Strategy {
public: 
    AllDefect() {
        name = "AllDefect";
        description = "Always defects";
    }

    char MakeMove(const std::vector<char>& my_history, const std::vector<char>& opp1_history, const std::vector<char>& opp2_history) override {
        return 'D';
    }
};

// 3
class Random : public Strategy {
public: 
    Random() {
        name = "Random";
        description = "Cooperates or defects randomly";
    }

    char MakeMove(const std::vector<char>& my_history, const std::vector<char>& opp1_history, const std::vector<char>& opp2_history) override {
        // std::srand(std::time(nullptr));
        return (std::rand() % 2 == 0) ? 'C' : 'D';
    }
};

// 4
class GoByMajority : public Strategy {
public: 
    GoByMajority() {
        name = "GoByMajority";
        description = "Cooperates or defects by majority";
    }

    char MakeMove(const std::vector<char>& my_history, const std::vector<char>& opp1_history, const std::vector<char>& opp2_history) override {
        if (opp1_history.empty() && opp2_history.empty()) {
            return 'C';
        }

        int total_defections = 0, total_cooperations = 0;
        for (char move: opp1_history) {
            if (move == 'D') {
                ++total_defections;
            } else {
                ++total_cooperations;
            }
        }

        for (char move: opp2_history) {
            if (move == 'D') {
                ++total_defections;
            } else {
                ++total_cooperations;
            }
        }

        return ((total_defections > total_cooperations) ? 'D' : 'C');
    }
};

// 5
// returns last move of enemy in front of itself
class TitForTat : public Strategy {
public: 
    TitForTat() {
        name = "TitForTat";
        description = "Returns last move of enemy in front of itself";
    }

    char MakeMove(const std::vector<char>& my_history, const std::vector<char>& opp1_history, const std::vector<char>& opp2_history) override {
        if (my_history.empty()) {
            return 'C';
        }
        return opp1_history.back();
    }
};

// TODO - MAKE SETCONFIG AND GETNAME METHODS FOR ALL STRATEGIES