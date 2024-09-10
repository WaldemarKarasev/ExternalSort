/*

    JSON settings for tape device and memory usage 
    Example:
    {
        "read_latency_ms":      100, // 100ms
        "write_latency_ms":     100, // 100ms   
        "move_latency_ms":      100, // 100ms
        "mem_usage_percentages": 90  // The size of the RAM avaliable for use by this process is 90% of the input tape size
    }

*/

// std
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <filesystem>


static std::string settings_path = "settings.config";

// user
#include <tape_file.hpp>

#include <settings.hpp>
extern nlohmann::json global_settings;

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cout << "Usage: e_sort input_tape output_tape" << std::endl;
        return EXIT_FAILURE;
    }

    global_settings = settings::ReadSettings();

    std::filesystem::path input_tape_path{argv[1]};
    std::filesystem::path output_tape_path{argv[2]};

    std::cout << "input_tape_path  - " << input_tape_path << std::endl;
    std::cout << "output_tape_path - " << output_tape_path << std::endl;

    // test
    io::TapeFile file("numbers.txt", std::ios::in | std::ios::out);
    
    io::TapeFile::buffer_type nums;

    file.Read(nums, 10);

    nums.push_back(7777);
    for (auto num : nums)
    {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    file.Write(nums, nums.size());

    // io::TapeFile::buffer_type nums2;
    // nums2.push_back(5555);
    // file.Write(nums2, nums2.size());


    // 1. Process input args

    // 2. Init IO_Device
    // 2.1 Read config file for IO_Device

    // 3. Batch source file into different batches in /tmp folder

    // 4. Sorting
    // 4.1 Sorting each batch
    // 4.2 External merge
    // 4.3 Output result filename
}