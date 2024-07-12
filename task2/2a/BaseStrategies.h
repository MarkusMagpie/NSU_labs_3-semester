// 1
#pragma once

#include "Strategy.h"

#include <cstdlib>
#include <ctime>

class PoorTrustingFool : public Strategy {
private:
    std::string name = "PoorTrustingFool";
    std::string description;
public: 
    PoorTrustingFool() {}
    char MakeMove(const std::vector<char>& my_history, const std::vector<char>& opp1_history, const std::vector<char>& opp2_history) override {
        return 'C';
    }

    void SetConfig(std::string key, std::string value) override {
        if (key == "Name") {
            name = value;
        }
        if (key == "Description") {
            description = value;
        }
    }

    std::string GetName() const override {
        return name;
    }

    // override is here for test purposes
    // https://ravesli.com/urok-164-modifikatory-override-i-final-kovariantnyj-tip-vozvrata/#toc-0
};

// 2
class AllDefect : public Strategy {
private:
    std::string name = "AllDefect";
    std::string description = "Always defects";
public: 
    AllDefect() {}
    char MakeMove(const std::vector<char>& my_history, const std::vector<char>& opp1_history, const std::vector<char>& opp2_history) override {
        return 'D';
    }

    void SetConfig(std::string key, std::string value) override {
        if (key == "Name") {
            name = value;
        }
        if (key == "Description") {
            description = value;
        }
    }

    std::string GetName() const override {
        return name;
    }
};

// 3
class Random : public Strategy {
private:
    std::string name = "Random";
    std::string description = "Cooperates or defects randomly";
public: 
    Random() {}
    char MakeMove(const std::vector<char>& my_history, const std::vector<char>& opp1_history, const std::vector<char>& opp2_history) override {
        // std::srand(std::time(nullptr));
        return (std::rand() % 2 == 0) ? 'C' : 'D';
    }

    void SetConfig(std::string key, std::string value) override {
        if (key == "Name") {
            name = value;
        }
        if (key == "Description") {
            description = value;
        }
    }

    std::string GetName() const override {
        return name;
    }
};

// 4
class GoByMajority : public Strategy {
private:
    std::string name = "GoByMajority";
    std::string description = "Cooperates or defects by majority";
public: 
    GoByMajority() {}
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

        // std::cout << "enemy1 history: ";
        // for (char move: opp1_history) {
        //     std::cout << move << " ";
        // }
        // std::cout << std::endl;

        // std::cout << "enemy2 history: ";
        // for (char move: opp2_history) {
        //     std::cout << move << " ";
        // }
        // std::cout << std::endl;

        return ((total_defections > total_cooperations) ? 'D' : 'C');
    }

    void SetConfig(std::string key, std::string value) override {
        if (key == "Name") {
            name = value;
        }
        if (key == "Description") {
            description = value;
        }
    }

    std::string GetName() const override {
        return name;
    }
};

// 5
// returns last move of enemy in front of itself
class TitForTat : public Strategy {
private:
    std::string name = "TitForTat";
    std::string description = "Returns last move of enemy in front of itself";
public: 
    TitForTat() {}
    char MakeMove(const std::vector<char>& my_history, const std::vector<char>& opp1_history, const std::vector<char>& opp2_history) override {
        if (my_history.empty()) {
            return 'C';
        }
        return opp1_history.back();
    }

    void SetConfig(std::string key, std::string value) override {
        if (key == "Name") {
            name = value;
        }
        if (key == "Description") {
            description = value;
        }
    }

    std::string GetName() const override {
        return name;
    }
};

// TODO - MAKE SETCONFIG AND GETNAME METHODS FOR ALL STRATEGIES