// Testing
#include <gtest/gtest.h>

// std
#include <fstream>
#include <string>
#include <vector>
#include <iostream>

// tape include 
#include <tape_file.hpp>

template <typename T>
void PrintFileState(T& file)
{
    std::cout << "p=" << file.tellp() << "; good=" << file.good() << "; eof=" << file.eof() << "; fail=" << file.fail() 
        << "; bad=" << file.bad() << "; rdtate=" << file.rdstate() << std::endl;
}


TEST(TapeFile, ReadJustWorks)
{
    std::string filename = "test_read.tape";

    std::string serialized_data = "1 2 3 -3 -2 -1 ";
    std::ofstream serialized_file(filename);
    
    if(serialized_file.is_open())
    {
        // PrintFileState(serialized_file);
        serialized_file.write(serialized_data.c_str(), serialized_data.size());
        serialized_file.flush();
        // PrintFileState(serialized_file);
    }
    io::TapeFile tape_file(filename);
    io::TapeFile::buffer_type tape_data;
    if (tape_file.IsOpen())
    {
        tape_file.Read(tape_data, 6);
    }
    EXPECT_EQ(tape_data.size(), 6);
    if (tape_data.size() < 6)
    {
        FAIL() << "Input file is empty";
    }
    EXPECT_EQ(tape_data[0],  1);
    EXPECT_EQ(tape_data[1],  2);
    EXPECT_EQ(tape_data[2],  3);
    EXPECT_EQ(tape_data[3], -3);
    EXPECT_EQ(tape_data[4], -2);
    EXPECT_EQ(tape_data[5], -1);    
}

TEST(TapeFile, WriteJustWorks)
{
    std::string filename = "test_write.tape";

    io::TapeFile write_file(filename, std::ios::out);
    io::TapeFile::buffer_type write_data = {1, 2, 3, -3, -2, -1};
    write_file.Write(write_data, write_data.size());

    io::TapeFile read_file(filename);
    io::TapeFile::buffer_type read_data;
    if (read_file.IsOpen())
    {
        read_file.Read(read_data, 6);
    }
    EXPECT_EQ(write_data.size(), read_data.size());
    
    for (std::size_t i = 0; i < read_data.size(); ++i)
    {
        EXPECT_EQ(write_data[i], read_data[i]);
    }
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}