#pragma once

// std
#include <filesystem>

// tape include
#include <tape_file.hpp>

namespace algo {

class Sorter
{

private:
    struct HeapNode
    {
        io::TapeFile::buffer_type::value_type value_;
        std::size_t index_;

        HeapNode(io::TapeFile::buffer_type::value_type value, std::size_t index)
            : value_(value)
            , index_(index) {}

        bool operator<(const HeapNode& rhs) const {
            return value_ < rhs.value_;
        }

        bool operator>(const HeapNode& rhs) const {
            return value_ > rhs.value_;
        }
    };

public:
    Sorter(std::filesystem::path temp_tapes_dir, std::size_t batch_size)
        : temp_tapes_dir_(std::move(temp_tapes_dir))
        , batch_size_(batch_size)
    {}

    std::filesystem::path Sort();

private:
    void Merge();

private:
    std::filesystem::path temp_tapes_dir_;
    std::filesystem::path output_sorted_file_path_;
    std::size_t batch_size_;
};

} // namespace algo