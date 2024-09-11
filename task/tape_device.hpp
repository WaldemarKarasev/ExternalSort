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

class MagnetTapeDevice
{
public:
    using buffer_type = std::vector<int>;
    using size_type = std::size_t;
    using streampos = size_type;

public:
    // Open/Close operations
    virtual void Open(std::ios::openmode mode = std::ios::in | std::ios::out) = 0;
    virtual void Open(std::filesystem::path, std::ios::openmode mode = std::ios::in | std::ios::out) = 0;
    virtual inline bool IsOpen() = 0;
    virtual void Close() = 0;

    virtual inline bool EndOfLine() = 0;

    // Magnet head operations
    virtual void MoveMagnetHeadRightRead() = 0;
    virtual void MoveMagnetHeadRightWrite() = 0;

    virtual void MoveMagnetHeadLeft() = 0;
    virtual void RewindMagnetHead(streampos pos = 0) = 0; // move seekp to std::ios::end in this stage of implementation, by default this funciton will rewind magnet head to the begining of the tape
    virtual size_type GetMagnetHeadPosition() = 0; // tell position of the magnet head

    // Cell operations
    virtual std::optional<int> ReadCurrentCell() = 0;
    virtual void WriteCurrentCell(int data) = 0;
};


class TapeDevice : public MagnetTapeDevice
{
public:
    TapeDevice(std::filesystem::path path, const nlohmann::json& settings_json);
    ~TapeDevice() { Close(); }

    TapeDevice(const TapeDevice&) = default;
    TapeDevice& operator=(const TapeDevice&) = default;

    TapeDevice(TapeDevice&&) = default;
    TapeDevice& operator=(TapeDevice&&) = default;

    // Open/Close operations
    virtual void Open(std::ios::openmode mode = std::ios::in | std::ios::out) override;
    virtual void Open(std::filesystem::path, std::ios::openmode mode = std::ios::in | std::ios::out) override;
    virtual inline bool IsOpen() override { return file_.is_open(); }
    virtual void Close() override;

    virtual inline bool EndOfLine() override { return file_.eof(); }

    // Magnet head operations
    virtual void MoveMagnetHeadRightRead() override;
    virtual void MoveMagnetHeadRightWrite() override;

    virtual void MoveMagnetHeadLeft() override;
    virtual void RewindMagnetHead(streampos pos = 0) override; // move seekp to std::ios::end in this stage of implementation, by default this funciton will rewind magnet head to the begining of the tape
    virtual size_type GetMagnetHeadPosition() override; // tell position of the magnet head

    // Cell operations
    virtual std::optional<int> ReadCurrentCell() override;
    virtual void WriteCurrentCell(int data) override;

private:
    // MagnetTapeDevice latency simulation
    std::chrono::milliseconds read_latency_{0};
    std::chrono::milliseconds write_letency_{0};
    std::chrono::milliseconds move_latency_{0};
    void ConfigureDevice(const nlohmann::json& settings_json); 
    void ReadLatency();
    void WriteLatency();
    void MoveLatency(size_type cell_count);

private:
    // utility/debug
    void PrintFileState();

private:
    // Through std::fstream implemented this TapeDevice
    std::filesystem::path path_;
    std::fstream file_;

    static constexpr char cell_delim_ = ' '; // for cell delimitation in file. All cells should be ended(closed) with this symbol
    size_type magnet_head_pos_{0}; // position of the magnet head on the tape
};

} // namespace io