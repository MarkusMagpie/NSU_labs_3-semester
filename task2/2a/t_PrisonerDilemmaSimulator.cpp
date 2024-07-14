#include "gtest/gtest.h"
#include "PrisonersDilemmaSimulator.h"
#include "BaseStrategies.h"

void RegisterBaseStrategies() {
    auto base_strat1 = []() { return std::make_unique<PoorTrustingFool>(); };
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

// default constructor or SetUp() function to prepare the objects for each test
// destructor or TearDown() function to release any resources you allocated in SetUp()
class PrisonersDilemmaSimulatorTest : public ::testing::Test {
protected:
    void SetUp() override {
        RegisterBaseStrategies();
    }

    void TearDown() override {};

    // member variables accessible in all test cases
    std::string config_dir = "configs";
    std::string matrix_file = "default_matrix.txt";
};

// 8 tests for all possible combinations of AllDefect and PoorTrustingFool
TEST_F(PrisonersDilemmaSimulatorTest, PoorTrustingFoolTest) {
    std::vector<std::string> strategies = { "PoorTrustingFool", "PoorTrustingFool", "PoorTrustingFool" };
    PrisonersDilemmaSimulator sim(strategies, 10, matrix_file, config_dir);
    sim.Run(false);

    const std::vector<int>& scores = sim.GetScores();
    const std::vector<std::vector<char>>& histories = sim.GetHistories();

    for (int score : scores) {
        EXPECT_EQ(score, 60); // предположим, что каждая стратегия набирает 60 очков под конец
    }

    for (const auto& history : histories) {
        for (char move : history) {
            EXPECT_EQ(move, 'C');
        }
    }
}

TEST_F(PrisonersDilemmaSimulatorTest, CCDTest) {
    std::vector<std::string> strategies = { "PoorTrustingFool", "PoorTrustingFool", "AllDefect" };
    PrisonersDilemmaSimulator sim(strategies, 10, matrix_file, config_dir);
    sim.Run(false);

    const std::vector<int>& scores = sim.GetScores();
    const std::vector<std::vector<char>>& histories = sim.GetHistories();

    // for (int score : scores) {
    //     std::cout << score << std::endl;
    // }

    EXPECT_EQ(scores[0], 30);
    EXPECT_EQ(scores[1], 30);
    EXPECT_EQ(scores[2], 100);

    // for (const auto& history : histories) {
    //     for (char move : history) {
    //         std::cout << move << " ";
    //     }
    //     std::cout << std::endl;
    // }
}

TEST_F(PrisonersDilemmaSimulatorTest, CDCTest) {
    std::vector<std::string> strategies = { "PoorTrustingFool", "AllDefect", "PoorTrustingFool" };
    PrisonersDilemmaSimulator sim(strategies, 10, matrix_file, config_dir);
    sim.Run(false);

    const std::vector<int>& scores = sim.GetScores();
    const std::vector<std::vector<char>>& histories = sim.GetHistories();

    EXPECT_EQ(scores[0], 30);
    EXPECT_EQ(scores[1], 100);
    EXPECT_EQ(scores[2], 30);
}

TEST_F(PrisonersDilemmaSimulatorTest, CDDTest) {
    std::vector<std::string> strategies = { "PoorTrustingFool", "AllDefect", "AllDefect" };
    PrisonersDilemmaSimulator sim(strategies, 10, matrix_file, config_dir);
    sim.Run(false);

    const std::vector<int>& scores = sim.GetScores();
    const std::vector<std::vector<char>>& histories = sim.GetHistories();
    
    EXPECT_EQ(scores[0], 0);
    EXPECT_EQ(scores[1], 60);
    EXPECT_EQ(scores[2], 60);
}

TEST_F(PrisonersDilemmaSimulatorTest, DCCTest) {
    std::vector<std::string> strategies = { "AllDefect", "PoorTrustingFool", "PoorTrustingFool" };
    PrisonersDilemmaSimulator sim(strategies, 10, matrix_file, config_dir);
    sim.Run(false);

    const std::vector<int>& scores = sim.GetScores();
    const std::vector<std::vector<char>>& histories = sim.GetHistories();
    
    EXPECT_EQ(scores[0], 100);
    EXPECT_EQ(scores[1], 30);
    EXPECT_EQ(scores[2], 30);
}

TEST_F(PrisonersDilemmaSimulatorTest, DCDTest) {
    std::vector<std::string> strategies = { "AllDefect", "PoorTrustingFool", "AllDefect" };
    PrisonersDilemmaSimulator sim(strategies, 10, matrix_file, config_dir);
    sim.Run(false);

    const std::vector<int>& scores = sim.GetScores();
    const std::vector<std::vector<char>>& histories = sim.GetHistories();
    
    EXPECT_EQ(scores[0], 60);
    EXPECT_EQ(scores[1], 0);
    EXPECT_EQ(scores[2], 60);
}

TEST_F(PrisonersDilemmaSimulatorTest, DDCTest) {
    std::vector<std::string> strategies = { "AllDefect", "AllDefect", "PoorTrustingFool" };
    PrisonersDilemmaSimulator sim(strategies, 10, matrix_file, config_dir);
    sim.Run(false);

    const std::vector<int>& scores = sim.GetScores();
    const std::vector<std::vector<char>>& histories = sim.GetHistories();
    
    EXPECT_EQ(scores[0], 60);
    EXPECT_EQ(scores[1], 60);
    EXPECT_EQ(scores[2], 0);
}

TEST_F(PrisonersDilemmaSimulatorTest, AllDefectTest) {
    std::vector<std::string> strategies = { "AllDefect", "AllDefect", "AllDefect" };
    PrisonersDilemmaSimulator sim(strategies, 10, matrix_file, config_dir);
    sim.Run(false);

    const std::vector<int>& scores = sim.GetScores();
    const std::vector<std::vector<char>>& histories = sim.GetHistories();

    for (int score : scores) {
        EXPECT_EQ(score, 20);
    }

    for (const auto& history : histories) {
        for (char move : history) {
            EXPECT_EQ(move, 'D');
        }
    }
}

// tests for GoByMajority
TEST_F(PrisonersDilemmaSimulatorTest, GoByMajority1Test) {
    std::vector<std::string> strategies = { "AllDefect", "AllDefect", "GoByMajority" };
    PrisonersDilemmaSimulator sim(strategies, 10, matrix_file, config_dir);
    sim.Run(false);

    const std::vector<int>& scores = sim.GetScores();
    const std::vector<std::vector<char>>& histories = sim.GetHistories();
    
    EXPECT_EQ(scores[0], 24);
    EXPECT_EQ(scores[1], 24);
    EXPECT_EQ(scores[2], 18);
}

TEST_F(PrisonersDilemmaSimulatorTest, GoByMajority2Test) {
    std::vector<std::string> strategies = { "AllDefect", "PoorTrustingFool", "GoByMajority" };
    PrisonersDilemmaSimulator sim(strategies, 10, matrix_file, config_dir);
    sim.Run(false);

    const std::vector<int>& scores = sim.GetScores();
    const std::vector<std::vector<char>>& histories = sim.GetHistories();
    
    EXPECT_EQ(scores[0], 100);
    EXPECT_EQ(scores[1], 30);
    EXPECT_EQ(scores[2], 30);
}

// tests for GoByMajority
TEST_F(PrisonersDilemmaSimulatorTest, TitForTat1Test) {
    std::vector<std::string> strategies = { "TitForTat", "AllDefect", "PoorTrustingFool" };
    // for 1st strat: 1: C; 2 - 10: D because 2nd strat is AllDefect
    PrisonersDilemmaSimulator sim(strategies, 10, matrix_file, config_dir);
    sim.Run(false);

    const std::vector<int>& scores = sim.GetScores();
    const std::vector<std::vector<char>>& histories = sim.GetHistories();
    
    EXPECT_EQ(scores[0], 57);
    EXPECT_EQ(scores[1], 64);
    EXPECT_EQ(scores[2], 3);
}

TEST_F(PrisonersDilemmaSimulatorTest, TitForTat2Test) {
    std::vector<std::string> strategies = { "AllDefect", "TitForTat", "PoorTrustingFool" };
    // for 1st strat: 1: C; 2 - 10: D because 2nd strat is AllDefect
    PrisonersDilemmaSimulator sim(strategies, 10, matrix_file, config_dir);
    sim.Run(false);

    const std::vector<int>& scores = sim.GetScores();
    const std::vector<std::vector<char>>& histories = sim.GetHistories();
    
    EXPECT_EQ(scores[0], 100);
    EXPECT_EQ(scores[1], 30);
    EXPECT_EQ(scores[2], 30);
}

TEST_F(PrisonersDilemmaSimulatorTest, TitForTat3Test) {
    std::vector<std::string> strategies = { "AllDefect", "PoorTrustingFool", "TitForTat" };
    // for 1st strat: 1: C; 2 - 10: D because 2nd strat is AllDefect
    PrisonersDilemmaSimulator sim(strategies, 10, matrix_file, config_dir);
    sim.Run(false);

    const std::vector<int>& scores = sim.GetScores();
    const std::vector<std::vector<char>>& histories = sim.GetHistories();
    
    EXPECT_EQ(scores[0], 64);
    EXPECT_EQ(scores[1], 3);
    EXPECT_EQ(scores[2], 57);
}

// tests for tournament game mode
TEST_F(PrisonersDilemmaSimulatorTest, TournamentFailTest) {
    std::vector<std::string> strategies = { "AllDefect", "PoorTrustingFool" };
    PrisonersDilemmaSimulator sim(strategies, 10, matrix_file, config_dir);
    
    EXPECT_THROW(sim.RunTournament(), std::runtime_error);

    try {
        sim.RunTournament();
    } catch (const std::runtime_error& e) {
        EXPECT_STREQ(e.what(), "Number of strategies must be 3 or more for tournament mode!");
        std::cout << e.what() << std::endl;
    } catch (...) {
        FAIL(); // FAIL means that we don't expect any other exceptions
    }
}



int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}