#include <gtest/gtest.h>

#include "Universe.h"

class UniverseTest : public ::testing::Test {
protected: 
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(UniverseTest, ConwayGameOfLifeGliderTest) {
    std::vector<std::vector<bool>> grid_initial = {
        {false, false, true}, 
        {true, false, true}, 
        {false, true, true}
    };

    Universe universe(3, 3, grid_initial);
    universe.SetName("Conway's Game of Life - with Glider");
    universe.SetRule("RLE: B3/S23");
    universe.tick();
    std::vector<std::vector<bool>> grid_expected = {
        {false, true, false},
        {false, false, true},
        {false, true, true}
    };

    EXPECT_EQ(universe.GetGrid(), grid_expected);
}

int main(void) {
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}