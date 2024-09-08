#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <filesystem>

namespace io {

class TapeDevice
{
public:
    using buffer_type = std::vector<int>;
    using size_type = std::size_t;

public:
    TapeDevice(std::filesystem::path path);
    void Open();
    void Read(buffer_type& dst, size_type size);
    void Write(buffer_type& src, size_type size);
    void Close();

    void Seekp(size_type pos); // return value void or should be *this as in the STL
    size_type Tellp();

    inline bool IsOpen() { return file_.is_open(); }

private:
    void WriteToFile(); // implementaion which requires extra space
    void ReadFromFile();  // implementaion which requires extra space

private:
    std::chrono::milliseconds read_latency_{0};
    std::chrono::milliseconds write_letency_{0};
    std::chrono::milliseconds move_latency_{0};
    // Latency simulation
    void ReadLatency();
    void WriteLatency();
    void MoveLatency(size_type cell_count);

private:
    // utility/debug
    void PrintFileState();

private:
    std::filesystem::path path_;
    std::fstream file_;

    buffer_type tape_buffer_;
    size_type magnet_head_pos_{0};
};

} // namespace io