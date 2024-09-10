#pragma once

// std
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <optional>

// nlohmann 
#include <nlohmann/json.hpp>


namespace io {

class TapeDevice
{
public:
    using buffer_type = std::vector<int>;
    using size_type = std::size_t;
    using streampos = size_type;


public:
    TapeDevice(std::filesystem::path path, const nlohmann::json& settings_json);
    ~TapeDevice() { Close(); }

    // Open/Close operations
    void Open(std::ios::openmode mode = std::ios::in | std::ios::out);
    void Open(std::filesystem::path, std::ios::openmode mode = std::ios::in | std::ios::out);
    inline bool IsOpen() { return file_.is_open(); }
    void Close();

    inline bool EndOfLine() { return file_.eof(); }

    // Magnet head operations
    void MoveMagnetHeadRightRead();
    void MoveMagnetHeadRightWrite();

    void MoveMagnetHeadLeft();
    void RewindMagnetHead(streampos pos = 0); // move seekp to std::ios::end in this stage of implementation, by default this funciton will rewind magnet head to the begining of the tape
    size_type GetMagnetHeadPosition(); // tell position of the magnet head

    // Cell operations
    std::optional<int> ReadCurrentCell();
    void WriteCurrentCell(int data);

private:
    // Service functions
    // ...

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

    static constexpr char cell_delim_ = ' '; // for cell delimitation in file. All cells should be ended(closed) with this symbol
    size_type magnet_head_pos_{0};
};

} // namespace io