#pragma once

#include "BaseStrategies.h"

#include <memory>

// 1
class GrimTrigger : public Strategy {
private:
    bool opponentDefected = false;
public:
    GrimTrigger() {
        name = "GrimTrigger";
        description = "Cooperates until the opponent defects. Once the opponent defects, it defects for the rest of the game"; 
    }

    char MakeMove(const std::vector<char>& myhistory, const std::vector<char>& opp1_history, const std::vector<char>& opp2_history) override {
        if (opponentDefected) return 'D';

        if (!myhistory.empty() && (opp1_history.back() == 'D' || opp2_history.back() == 'D')) {
            opponentDefected = true;
            return 'D';
        }

        return 'C';
    }
};

// 2 meta-strat
class StrategySwitcher : public Strategy {
private:
    std::vector<std::unique_ptr<Strategy>> strategies;
    int cur_strat_index = -1;
    int round = -1;
    const int switch_interval = 2;
public:
    StrategySwitcher() {
        name = "StrategySwitcher";
        description = "Switches strategy every few rounds";

        strategies.push_back(std::make_unique<AllDefect>());
        strategies.push_back(std::make_unique<TitForTat>());
        strategies.push_back(std::make_unique<GoByMajority>());
    }

    char MakeMove(const std::vector<char>& myhistory, const std::vector<char>& opp1_history, const std::vector<char>& opp2_history) override {
        ++round;
        if (round % switch_interval == 0) {
            cur_strat_index = (cur_strat_index + 1) % strategies.size();
        }

        std::cout << "\n(Switcher's current strategy: " << strategies[cur_strat_index]->GetName() << ")" << std::endl;
        return strategies[cur_strat_index]->MakeMove(myhistory, opp1_history, opp2_history);
    }
};
