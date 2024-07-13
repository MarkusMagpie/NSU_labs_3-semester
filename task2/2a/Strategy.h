#pragma once

#include <vector>
#include <string>

class Strategy {
protected: // protected for child classes; https://ravesli.com/urok-157-nasledovanie-i-spetsifikatory-dostupa-protected/
    std::string name;
    std::string description;
public:
    virtual ~Strategy() = default;
    
    virtual char MakeMove(const std::vector<char>& my_history, 
    const std::vector<char>& opp1_history, 
    const std::vector<char>& opp2_history) = 0;

    virtual void SetConfig(std::string key, std::string value) {
        if (key == "Name") {
            name = value;
        }
        if (key == "Description") {
            description = value;
        }
    }

    virtual std::string GetName() const { return name; }
};

// functions here are 'pure virtual' => any child class must provide implementations for these two functions
// A class that contains at least one pure virtual function (i have 1) is considered an abstract class.
//      Another child class from it mustprovide implementations for both pure virtual functions