#pragma once 

#include "PrisonersDilemmaSimulator.h"
#include "BaseStrategies.h"
#include "CustomStrategies.h"

#include <map>
#include <string>

std::map<std::string, std::string> ParseCommandLine(int argc, char* argv[]);

// used lambda function here
// each lambda has the type std::function<std::unique_ptr<Strategy>()>
// RegisterStrategy's 2nd argument type: std::function<std::unique_ptr<Strategy>()>

// https://metanit.com/cpp/tutorial/15.4.php
void RegisterBaseStrategies() {
    std::function<std::unique_ptr<Strategy>()> base_strat1 = []() { return std::make_unique<PoorTrustingFool>(); };
    auto base_strat2 = []() { return std::make_unique<AllDefect>(); };
    auto base_strat3 = []() { return std::make_unique<Random>(); };
    auto base_strat4 = []() { return std::make_unique<GoByMajority>(); };
    auto base_strat5 = []() { return std::make_unique<TitForTat>(); };

    StrategyFactory::GetInstance().RegisterStrategy("PoorTrustingFool", base_strat1);
    StrategyFactory::GetInstance().RegisterStrategy("AllDefect", base_strat2);
    StrategyFactory::GetInstance().RegisterStrategy("Random", base_strat3);
    StrategyFactory::GetInstance().RegisterStrategy("GoByMajority", base_strat4);
    StrategyFactory::GetInstance().RegisterStrategy("TitForTat", base_strat5);
}

void RegisterCustomStrategies() {
    auto custom_strat1 = []() { return std::make_unique<GrimTrigger>(); };
    auto custom_strat2 = []() { return std::make_unique<StrategySwitcher>(); };

    StrategyFactory::GetInstance().RegisterStrategy("GrimTrigger", custom_strat1);
    StrategyFactory::GetInstance().RegisterStrategy("StrategySwitcher", custom_strat2);
}

void CheckArguments(int argc, char* argv[]) {
    if (argc < 4) {
        throw std::invalid_argument("Error: not enough arguments passed to run any simulation mode");
    }
}

int main(int argc, char* argv[]);