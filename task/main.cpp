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

// user
#include <tape_file.hpp>
#include <settings.hpp>
#include <tape_splitter.hpp>
#include <sorter.hpp>

int main(int argc, char* argv[])
{
    // 1. Process input args
    if (argc < 3)
    {
        std::cerr << "Usage: sort input_tape output_tape" << std::endl;
        return EXIT_FAILURE;
    }
    std::filesystem::path input_tape_path = std::filesystem::current_path() / argv[1];
    std::filesystem::path output_tape_path = std::filesystem::current_path() / argv[2];

    // 2. Init IO_Device
    // 2.1 Read config file for IO_Device
    settings::global = settings::ReadSettings();
    // std::cout << "settings - " << settings::global.dump(4) << std::endl;

    // 3. Batch source file into different batches in /tmp folder
    service::TapeSplitter splitter(input_tape_path, settings::global["mem_usage_percentages"]);
    auto[tmp_tape_path, batch_size] = splitter.Split();

    // 4. Sorting
    algo::Sorter sorter(tmp_tape_path, output_tape_path, batch_size);
    // 4.1 Sorting each batch
    // 4.2 External merge
    sorter.Sort();

    return EXIT_SUCCESS;
}