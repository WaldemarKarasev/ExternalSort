// std
#include <vector>


// Testing
#include <gtest/gtest.h>

// tape includes

#include <test/tape_file_test.hpp> // File interface implementation for testing external merge algorithm 

#include <sorter.hpp>


using TapeFile = io::test::TapeFileTest;

TEST(External_Sort, JustWorks)
{
    TapeFile file({-2, 3, 1, 5, 8, 43, 2, 6, 9});

    algo::Sorter().Sort();


}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}