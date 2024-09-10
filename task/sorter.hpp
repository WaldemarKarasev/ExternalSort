#pragma once

// std
#include <filesystem>

// tape include


namespace algo {

class Sorter
{
public:
    Sorter(std::filesystem::path temp_tapes_dir, std::size_t batch_size)
        : temp_tapes_dir_(std::move(temp_tapes_dir))
        , batch_size_(batch_size)
    {}

    std::filesystem::path Sort();

private:
    std::filesystem::path temp_tapes_dir_;
    std::size_t batch_size_;
};

} // namespace algo