#pragma once

// std
#include <filesystem>
#include <tuple>

// tape include
#include <tape_file.hpp>


namespace service {

class TapeSplitter
{
public:
    TapeSplitter(std::filesystem::path& input_file, std::size_t ram_percentage);
    std::tuple<std::filesystem::path, std::size_t> Split();

private:
    std::size_t GetInputTapeSize();

private:
    std::size_t ram_percentage_{50}; // default value is 50% of input file(tape)
    std::filesystem::path input_file_path_;
    std::filesystem::path tmp_tape_path_;
    std::size_t input_tape_file_size_;
};

} // namespace service