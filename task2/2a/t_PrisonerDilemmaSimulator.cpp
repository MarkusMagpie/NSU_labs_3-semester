#include "gtest/gtest.h"
#include "PrisonersDilemmaSimulator.h"
#include "BaseStrategies.h"

TEST(PrisonersDilemmaSimulatorTest, AlwaysCooperateTest) {
    PoorTrustingFool strategy;
    std::vector<char> history;
    EXPECT_EQ('C', strategy.MakeMove(history, history, history));
}

TEST(PrisonersDilemmaSimulatorTest, AlwaysDefectTest) {
    AllDefect strategy;
    std::vector<char> history;
    EXPECT_EQ('D', strategy.MakeMove(history, history, history));
}

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}