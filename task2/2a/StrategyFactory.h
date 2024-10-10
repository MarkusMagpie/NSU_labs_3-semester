#pragma once

#include "Strategy.h"
#include <string>
#include <memory>
#include <functional> // for std::function
#include <map>

class StrategyFactory {
public:
    // Define a function that returns a unique_ptr to a Strategy object
    using StrategyCreator = std::function<std::unique_ptr<Strategy>()>;

    // return a reference to the static instance of factory (singleton)
    // метод GetInstance возвращает ссылку на единственный экземпляр StrategyFactory
    // singleton - a class that has only one instance. This instance - is global point of access to this class
    static StrategyFactory& GetInstance() {
        static StrategyFactory instance;
        return instance;
    }

    // registration of new strategies with name and creator (creator - function that returns a unique_ptr to a Strategy object)
    void RegisterStrategy(const std::string& name, StrategyCreator creator) {
        creators[name] = creator;
    }

    // creation of new stratefy by name
    std::unique_ptr<Strategy> CreateStrategy(const std::string& name) {
        if (creators.find(name) != creators.end()) { // check if name is in the map, if yes, we create a new strategy
            return creators[name]();
        }
        return nullptr;
    }

private:
    std::map<std::string, StrategyCreator> creators;
    // keys - names of strategies
    // values - function that returns a unique_ptr to a Strategy object (StrategyCreator)
    // https://en.cppreference.com/w/cpp/container/unordered_map
};

// How to use? 
// 1 call GetInstance() to get a reference to the static instance of StrategyFactory
// 2 call RegisterStrategy() to register new strategy

// Example:
// std::function<std::unique_ptr<Strategy>()> base_strat1 = []() { return std::make_unique<PoorTrustingFool>(); };
// StrategyFactory::GetInstance().RegisterStrategy("PoorTrustingFool", base_strat1);