#include <gtest/gtest.h>

#include "Universe.h"

class UniverseTest : public ::testing::Test {
protected: 
    void SetUp() override {}
    void TearDown() override {}
};

// still life 2 tests
TEST_F(UniverseTest, ConwayGameOfLifeBeehiveTest) {
    std::vector<std::vector<bool>> grid_initial = {
        {0, 1, 1, 0}, 
        {1, 0, 0, 1}, 
        {0, 1, 1, 0}
    };

    Universe universe(grid_initial[0].size(), grid_initial.size(), grid_initial);
    universe.SetName("Conway's Game of Life - with Bee-Hive");
    universe.SetRule("B3/S23");

    std::cout << "Name: "<< universe.GetName() << std::endl;
    std::cout << "Rule: " << universe.GetRule() << std::endl;
    std::cout << "-----BEFORE TICK-----" << std::endl;
    universe.print();
    universe.tick();
    std::cout << "-----AFTER TICK-----" << std::endl;
    universe.print();

    std::vector<std::vector<bool>> grid_expected = {
        {0, 1, 1, 0}, 
        {1, 0, 0, 1}, 
        {0, 1, 1, 0}
    };

    EXPECT_EQ(universe.GetGrid(), grid_expected);
}

TEST_F(UniverseTest, ConwayGameOfLifeBoatTest) {
    std::vector<std::vector<bool>> grid_initial = {
        {1, 1, 0}, 
        {1, 0, 1}, 
        {0, 1, 0}
    };

    Universe universe(grid_initial[0].size(), grid_initial.size(), grid_initial);
    universe.SetName("Conway's Game of Life - with Boat");
    universe.SetRule("B3/S23");

    std::cout << "Name: "<< universe.GetName() << std::endl;
    std::cout << "Rule: " << universe.GetRule() << std::endl;
    std::cout << "-----BEFORE TICK-----" << std::endl;
    universe.print();
    universe.tick();
    std::cout << "-----AFTER TICK-----" << std::endl;
    universe.print();

    std::vector<std::vector<bool>> grid_expected = {
        {1, 1, 0}, 
        {1, 0, 1}, 
        {0, 1, 0}
    };

    EXPECT_EQ(universe.GetGrid(), grid_expected);
}

// oscillator tests
TEST_F(UniverseTest, ConwayGameOfLifeBeaconTest) {
    std::vector<std::vector<bool>> grid_initial = {
        {1, 1, 0, 0}, 
        {1, 1, 0, 0}, 
        {0, 0, 1, 1}, 
        {0, 0, 1, 1}
    };

    Universe universe(grid_initial[0].size(), grid_initial.size(), grid_initial);
    universe.SetName("Conway's Game of Life - with Beacon");
    universe.SetRule("B3/S23");
    std::cout << "Name: "<< universe.GetName() << std::endl;
    std::cout << "Rule: " << universe.GetRule() << std::endl;
    std::cout << "-----BEFORE TICK-----" << std::endl;
    universe.print();
    
    std::cout << "-----AFTER 1 TICK-----" << std::endl;
    universe.tick();
    universe.print();

    std::vector<std::vector<bool>> grid_one_tick = {
        {1, 1, 0, 0}, 
        {1, 0, 0, 0}, 
        {0, 0, 0, 1}, 
        {0, 0, 1, 1}
    };

    EXPECT_EQ(universe.GetGrid(), grid_one_tick);

    std::cout << "\n-----AFTER 2 TICKS-----" << std::endl;
    universe.tick();
    universe.print();

    std::vector<std::vector<bool>> grid_two_tick = {
        {1, 1, 0, 0}, 
        {1, 1, 0, 0}, 
        {0, 0, 1, 1}, 
        {0, 0, 1, 1}
    };

    EXPECT_EQ(universe.GetGrid(), grid_two_tick);
}

TEST_F(UniverseTest, ConwayGameOfLifePulsarTest) {
    std::vector<std::vector<bool>> grid_initial = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0},
        {0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0},
        {0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0},
        {0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0},
        {0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0},
        {0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };
    Universe universe(15, 15, grid_initial);
    universe.SetName("Conway's Game of Life - Pulsar");
    universe.SetRule("B3/S23");

    std::cout << "Name: "<< universe.GetName() << std::endl;
    std::cout << "Rule: " << universe.GetRule() << std::endl;
    std::cout << "-----BEFORE TICK-----" << std::endl;
    universe.print();

    std::cout << "\n-----AFTER 1 TICK-----" << std::endl;
    universe.tick();
    universe.print();

    std::vector<std::vector<bool>> grid_one_tick = {
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1},
        {0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0},
        {0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0},
        {0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0},
        {1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0}
    };

    EXPECT_EQ(universe.GetGrid(), grid_one_tick);

    std::cout << "\n-----AFTER 2 TICKS-----" << std::endl;
    universe.tick();
    universe.print();

    std::vector<std::vector<bool>> grid_two_tick = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0},
        {0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0},
        {0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0},
        {0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0},
        {0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0},
        {0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0},
        {0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0},
        {0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0},
        {0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    EXPECT_EQ(universe.GetGrid(), grid_two_tick);

    std::cout << "\n-----AFTER 3 TICKS-----" << std::endl;
    universe.tick();
    universe.print();

    std::vector<std::vector<bool>> grid_three_tick = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0},
        {0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0},
        {0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0},
        {0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0},
        {0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0},
        {0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    EXPECT_EQ(universe.GetGrid(), grid_three_tick);
}

// spaceship tests(2)
TEST_F(UniverseTest, ConwayGameOfLifeGliderTest) {
    std::vector<std::vector<bool>> grid_initial = {
        {0, 0, 1, 0}, 
        {1, 0, 1, 0}, 
        {0, 1, 1, 0},
        {0, 0, 0, 0}
    };

    Universe universe(grid_initial[0].size(), grid_initial.size(), grid_initial);
    universe.SetName("Conway's Game of Life - with Glider");
    universe.SetRule("B3/S23");

    std::cout << "Name: "<< universe.GetName() << std::endl;
    std::cout << "Rule: " << universe.GetRule() << std::endl;
    std::cout << "-----BEFORE TICK-----" << std::endl;
    universe.print();
    universe.tick();
    std::cout << "-----AFTER TICK-----" << std::endl;
    universe.print();

    std::vector<std::vector<bool>> grid_expected = {
        {0, 1, 0, 0},
        {0, 0, 1, 1},
        {0, 1, 1, 0},
        {0, 0, 0, 0}
    };

    EXPECT_EQ(universe.GetGrid(), grid_expected);
}

TEST_F(UniverseTest, ConwayGameOfLifeLWSSTest) {
    std::vector<std::vector<bool>> grid_initial = {
        {0, 0, 0, 0, 1, 0, 0, 1}, 
        {0, 0, 0, 1, 0, 0, 0, 0}, 
        {0, 0, 0, 1, 0, 0, 0, 1},
        {0, 0, 0, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0}
    };

    Universe universe(grid_initial[0].size(), grid_initial.size(), grid_initial);
    universe.SetName("Conway's Game of Life - with Lightweight Spaceship (LWSS)");
    universe.SetRule("B3/S23");

    std::cout << "Name: "<< universe.GetName() << std::endl;
    std::cout << "Rule: " << universe.GetRule() << std::endl;
    std::cout << "-----BEFORE TICK-----" << std::endl;
    universe.print();
    
    std::cout << "-----AFTER 1 TICK-----" << std::endl;
    universe.tick();
    universe.print();

    std::vector<std::vector<bool>> grid_one_tick = {
        {0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 1, 1, 0, 0, 0}, 
        {0, 0, 1, 1, 0, 1, 1, 0},
        {0, 0, 0, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 1, 1, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0}
    };

    EXPECT_EQ(universe.GetGrid(), grid_one_tick);

    std::cout << "-----AFTER 2 TICKS-----" << std::endl;
    universe.tick();
    universe.print();

    std::vector<std::vector<bool>> grid_two_tick = {
        {0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 1, 1, 1, 1, 0, 0}, 
        {0, 0, 1, 0, 0, 0, 1, 0},
        {0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0}
    };

    EXPECT_EQ(universe.GetGrid(), grid_two_tick);

    std::cout << "-----AFTER 3 TICKS-----" << std::endl;
    universe.tick();
    universe.print();

    std::vector<std::vector<bool>> grid_three_tick = {
        {0, 0, 0, 1, 1, 0, 0, 0}, 
        {0, 0, 1, 1, 1, 1, 0, 0}, 
        {0, 1, 1, 0, 1, 1, 0, 0},
        {0, 0, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0}
    };

    EXPECT_EQ(universe.GetGrid(), grid_three_tick);
}

int main(void) {
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}