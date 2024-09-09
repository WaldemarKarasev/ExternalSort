#pragma once

// std
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>

// nlohmann 
#include <nlohmann/json.hpp>


namespace io {

class TapeDevice
{
public:
    using buffer_type = std::vector<int>;
    using size_type = std::size_t;

public:
    TapeDevice(std::filesystem::path path, nlohmann::json settings_json);
    void Open();
    void Read(buffer_type& dst, size_type size);
    void Write(buffer_type& src, size_type size);
    void Close();

    void Seekp(size_type pos); // setting magnet head to specific position in file
    size_type Tellp(); // tell position of the magnet head

    inline bool IsOpen() { return file_.is_open(); }


    void Open();
    void Close();
    void MoveMagnetHeadRight();
    void MoveMagnetHeadLeft();
    void ReqindMagnetHead(); // move seekp to std::ios::end in this stage of implementation
    int ReadCurrentCell();
    void WriteCurrentCell(int data);

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
    void ConfigureDevice(const nlohmann::json& settings_json); 

private:
    std::filesystem::path path_;
    std::fstream file_;

    buffer_type tape_buffer_;
    size_type magnet_head_pos_{0};

    nlohmann::json settings_json_;
};

} // namespace io