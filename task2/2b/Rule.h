#pragma once

#include <iostream>
#include <set>
#include <sstream>

class Rule {
private:
    std::set<int> birth_condition;
    std::set<int> survival_condition;
    std::string rule_str;

    void ParseRule(const std::string& rule_str);
public:
    Rule();
    Rule(const std::string& rule_str);
    std::string GetRuleString() const { return rule_str; }

    bool IsBirth(int neighbors) const { return birth_condition.count(neighbors) > 0; }
    bool IsSurvival(int neighbors) const { return survival_condition.count(neighbors) > 0; }
};

Rule::Rule() : birth_condition(), survival_condition(), rule_str("") {}

Rule::Rule(const std::string& rule_str) : rule_str(rule_str) {
    ParseRule(rule_str);
}

void Rule::ParseRule(const std::string& rule_str) {
    std::istringstream stream(rule_str);
    std::string part;
    while (std::getline(stream, part, '/')) {
        if (part[0] == 'B') {
            for (char c : part.substr(1)) {
                birth_condition.insert(c - '0'); // - '0' to convert char to int
            }
        } else if (part[0] == 'S') {
            for (char c : part.substr(1)) {
                survival_condition.insert(c - '0');
            }
        }
    }
}