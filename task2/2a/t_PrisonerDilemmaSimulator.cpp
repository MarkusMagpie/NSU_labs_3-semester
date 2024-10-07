#include "gtest/gtest.h"
// #include "PrisonersDilemmaSimulator.h"
// #include "BaseStrategies.h"
#include "main.h"

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

TEST_F(PrisonersDilemmaSimulatorTest, MainExceptionTest) {
    char* argv[] = { (char*)"name", (char*)"arg1", (char*)"arg2" };
    int argc = 3;

    EXPECT_THROW(CheckArguments(argc, argv), std::invalid_argument);

    try {
        CheckArguments(argc, argv);
    } catch (const std::invalid_argument& e) {
        EXPECT_STREQ(e.what(), "Error: not enough arguments passed to run any simulation mode");
        std::cout << e.what() << std::endl;
    } catch (...) {
        FAIL(); // FAIL means that we don't expect any other exceptions =>  if catch any other exception, it's a fail
    }
}

// 2 tests for long/fast exceptons
// Number of strategies must be 3 for fast (or long) mode!
TEST_F(PrisonersDilemmaSimulatorTest, FastExceptionTest) {
    std::vector<std::string> strategies = { "AllDefect", "AllDefect" };
    PrisonersDilemmaSimulator sim(strategies, 10, matrix_file, config_dir);

    try {
        sim.Run();
    } catch (const std::runtime_error& e) {
        EXPECT_STREQ(e.what(), "Number of strategies must be 3 for fast (or long) mode!");
        std::cout << e.what() << std::endl;
    } catch (...) {
        FAIL();
    }

    // EXPECT_THROW(sim.Run(), std::runtime_error);
}

TEST_F(PrisonersDilemmaSimulatorTest, LongExceptionTest) {
    std::vector<std::string> strategies = { "AllDefect", "AllDefect", "AllDefect", "AllDefect" };
    PrisonersDilemmaSimulator sim(strategies, 10, matrix_file, config_dir);

    try {
        sim.Run();
    } catch (const std::runtime_error& e) {
        EXPECT_STREQ(e.what(), "Number of strategies must be 3 for fast (or long) mode!");
        std::cout << e.what() << std::endl;
    } catch (...) {
        FAIL();
    }

    // EXPECT_THROW(sim.Run(true), std::runtime_error);
}

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

    EXPECT_EQ(scores[0], 30);
    EXPECT_EQ(scores[1], 30);
    EXPECT_EQ(scores[2], 100);
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
TEST_F(PrisonersDilemmaSimulatorTest, TournamentExceptionTest) {
    std::vector<std::string> strategies = { "AllDefect", "PoorTrustingFool" };
    PrisonersDilemmaSimulator sim(strategies, 10, matrix_file, config_dir);

    try {
        sim.RunTournament();
    } catch (const std::runtime_error& e) {
        EXPECT_STREQ(e.what(), "Number of strategies must be 3 or more for tournament mode!");
        std::cout << e.what() << std::endl;
    } catch (...) {
        FAIL(); // FAIL means that we don't expect any other exceptions
    }
}

TEST_F(PrisonersDilemmaSimulatorTest, TournamentEqualScoresTest) {
    std::vector<std::string> strategies = { "PoorTrustingFool", "PoorTrustingFool", "PoorTrustingFool", "PoorTrustingFool" };
    PrisonersDilemmaSimulator sim(strategies, 10, matrix_file, config_dir);
    sim.RunTournament();
    
    const auto& tournament_scores = sim.GetTournamentScores(); // new getter function here
    
    int first_score = tournament_scores.begin()->second;
    for (const auto& entry : tournament_scores) {
        EXPECT_EQ(entry.second, first_score);
    }
}

TEST_F(PrisonersDilemmaSimulatorTest, TournamentMixedStrategiesTest) {
    std::vector<std::string> strategies = { "PoorTrustingFool", "AllDefect", "TitForTat", "Random" };
    PrisonersDilemmaSimulator sim(strategies, 10, matrix_file, config_dir);
    sim.RunTournament();
    
    const auto& tournament_scores = sim.GetTournamentScores();
    const auto& tournament_histories = sim.GetTournamentHistories();
    
    // check if strategies have non-zero scores and that histories are recorded
    for (const auto& entry : tournament_scores) {
        EXPECT_GT(entry.second, 0); // check that entry.second > 0
    }

    for (const auto& entry : tournament_histories) {
        const auto& history = entry.second;
        int non_empty_count = 0;
        for (const auto& action : history) {
            if (!action.empty()) {
                non_empty_count++;
            }
        }

        // std::cout << "Non-empty vectors count: " << non_empty_count << std::endl;
        EXPECT_EQ(non_empty_count, 3);
    }
}

// tests for detailed mode
TEST_F(PrisonersDilemmaSimulatorTest, DetailedModeExceptionTest) {
    std::vector<std::string> strategies = { "PoorTrustingFool", "TitForTat" }; // number of strategies must be precisely 3
    PrisonersDilemmaSimulator sim(strategies, 5, matrix_file, config_dir);  

    EXPECT_THROW(sim.RunDetailed(), std::runtime_error); 
}

TEST_F(PrisonersDilemmaSimulatorTest, DetailedModeQuitTest) {
    std::vector<std::string> strategies = { "PoorTrustingFool", "TitForTat", "AllDefect" };
    PrisonersDilemmaSimulator sim(strategies, 5, matrix_file, config_dir);

    // CiN PLAN: 1 save cin buffer; 
    // 2 create istringstream object; 
    // 3 redirect cin to istringstream object; 
    // 4 run detailed simulation; 
    // 5 restore cin buffer
    std::streambuf* orig_cin = std::cin.rdbuf(); // std::cin.rdbuf() returns pointer to std::cin buffer/ save original cin buffer
    std::streambuf* orig_cout = std::cout.rdbuf();
    std::istringstream input("quit\n"); // simulate user input: "quit"
    std::ostringstream output;

    std::cin.rdbuf(input.rdbuf()); // redirect std::cin to read from input stream
    std::cout.rdbuf(output.rdbuf());

    sim.RunDetailed();

    // Reset cin and cout
    std::cin.rdbuf(orig_cin); // restore std::cin buffer
    std::cout.rdbuf(orig_cout);

    std::string expected_output = "Simulation interrupted by user.\n";
    EXPECT_NE(output.str().find(expected_output), std::string::npos); // check that output contains expected_output
}

TEST_F(PrisonersDilemmaSimulatorTest, DetailedModeMoveTest) {
    std::vector<std::string> strategies = { "PoorTrustingFool", "TitForTat", "AllDefect" };
    PrisonersDilemmaSimulator sim(strategies, 1, matrix_file, config_dir);

    std::streambuf* orig_cin = std::cin.rdbuf();
    std::streambuf* orig_cout = std::cout.rdbuf();
    std::istringstream input("enter\nquit\n");
    std::ostringstream output;
    std::cin.rdbuf(input.rdbuf());
    std::cout.rdbuf(output.rdbuf());

    sim.RunDetailed();

    std::cin.rdbuf(orig_cin);
    std::cout.rdbuf(orig_cout);

    const std::vector<int>& scores = sim.GetScores();
    EXPECT_EQ(scores[0], 3); // PoorTrustingFool
    EXPECT_EQ(scores[1], 3); // TitForTat
    EXPECT_EQ(scores[2], 10); // AllDefect
}

// matrix exception tests
TEST_F(PrisonersDilemmaSimulatorTest, LoadMatrixFileNotFoundTest) {
    std::vector<std::string> strategies = { "PoorTrustingFool", "TitForTat", "AllDefect" };

    try {
        PrisonersDilemmaSimulator sim(strategies, 1, "nonexistent_file.txt", config_dir);
        sim.RunTournament();

    } catch (const std::runtime_error& e) {
        EXPECT_STRNE(e.what(), "");
        EXPECT_STRCASEEQ(e.what(), "Failed to open matrix file: nonexistent_file.txt");
    }
}

// config exception tests
TEST_F(PrisonersDilemmaSimulatorTest, LoadStrategyConfigsFileNotFoundTest) {
    std::vector<std::string> strategies = { "PoorTrustingFool", "PoorTrustingFool", "PoorTrustingFool" };

    try {
        PrisonersDilemmaSimulator sim(strategies, 1, "default_matrix.txt", "nonexistent_config_dir");
        sim.LoadStrategyConfigs("nonexistent_config_dir");
        FAIL() << "Expected std::runtime_error to be thrown";
    } catch (const std::runtime_error& e) {
        EXPECT_STRNE(e.what(), "");
        EXPECT_STRCASEEQ(e.what(), "Failed to open config file: nonexistent_config_dir/PoorTrustingFool.txt");
    }
}

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}