#include <tape_device.hpp>

#include <iostream>
#include <thread>
#include <cmath>

namespace io {

TapeDevice::TapeDevice(std::filesystem::path path, const nlohmann::json& settings_json)
    : path_{std::move(path)}
{
    ConfigureDevice(settings_json);
}

void TapeDevice::Open(std::ios::openmode mode)
{
    file_.open(path_, mode);
}
void TapeDevice::Open(std::filesystem::path path, std::ios::openmode mode)
{
    path_ = std::move(path);
    Open(mode);
}
void TapeDevice::Close()
{
    file_.close();

    // When closing device magnet head moves to the begining of the Tape (moves to index 0))
    MoveLatency(magnet_head_pos_);
}

// Magnet head operations
void TapeDevice::MoveMagnetHeadRightRead()
{
    char number[20];
    file_.getline(number, sizeof(number), cell_delim_);

    auto prev_pos = file_.tellp();
    file_.get(); // try eof ????
    if (file_.eof())
    {
        PrintFileState();
        file_.clear();
        file_.setstate(std::ios_base::eofbit);
        PrintFileState();
    }
    else
    {
        file_.seekp(prev_pos);
        PrintFileState();
        ++magnet_head_pos_;
    }

    MoveLatency(1);
    // std::cout << "MoveMagnetHeadRight() - end" << std::endl;
}

void TapeDevice::MoveMagnetHeadRightWrite()
{
    char number[20];
    file_.getline(number, sizeof(number), cell_delim_);

    if (file_.eof())
    {
        file_.clear();
        file_.seekp(0, std::ios::end);
    }
    ++magnet_head_pos_;

    MoveLatency(1);
    // std::cout << "MoveMagnetHeadRight() - end" << std::endl;
}


TapeDevice::size_type TapeDevice::GetMagnetHeadPosition() // tell position of the magnet head
{
    return magnet_head_pos_;
}

// Cell operations
std::optional<int> TapeDevice::ReadCurrentCell()
{
    // std::cout << "ReadCurrentCell()" << std::endl;

    if (file_.eof())
    {
        return std::nullopt;
    }
    
    // return value
    std::optional<int> cell_value;

    // Remembering prev std::fstream streampos for current "cell"
    auto prev_pos = file_.tellp();

    char number[20];
    file_.getline(number, sizeof(number), cell_delim_);
    try {
        cell_value = std::stoi(number);
        std::cout << "cell_str=\"" << number << "\"; cell_value=" << cell_value.value() << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << "cell_str - \"" << number << "\"; " <<  e.what() << '\n';
    }

    // Moving back streampos for std::fstream back for correct logic
    file_.seekp(prev_pos);

    ReadLatency();

    return cell_value;
}
void TapeDevice::WriteCurrentCell(int data)
{
    // std::cout << "WriteCurrentCell() - start" << std::endl;

    // Remembering prev std::fstream streampos for current "cell"
    auto prev_pos = file_.tellp();
    
    // Writing serialized data into std::fstream
    std::cout << "TapeDevice::WriteCurrentCell(" << std::to_string(data) << ")" << std::endl;
    std::string serialized_data = std::to_string(data) + " ";
    file_.write(serialized_data.c_str(), serialized_data.size());


    // Moving back streampos for std::fstream back for correct logic
    file_.seekp(prev_pos);
    
    WriteLatency();
    // std::cout << "WriteCurrentCell() - end" << std::endl;
}

static void Wait(std::chrono::milliseconds wait_time)
{
    std::this_thread::sleep_for(wait_time);
}

void TapeDevice::ReadLatency()
{
    Wait(read_latency_);
}

void TapeDevice::WriteLatency()
{
    Wait(write_letency_);   
}

void TapeDevice::MoveLatency(size_type cell_count)
{
    Wait(move_latency_ * cell_count);
}

void TapeDevice::PrintFileState()
{
    std::cout << "p=" << file_.tellp() << "; good=" << file_.good() << "; eof=" << file_.eof() << "; fail=" << file_.fail()
    << "; bad=" << file_.bad() << "; rdtate=" << file_.rdstate() << std::endl;
}

void TapeDevice::ConfigureDevice(const nlohmann::json& settings_json)
{
    if (settings_json.contains("read_latency") && 
        !settings_json["read_latency"].is_null() && 
          settings_json["read_latency"].is_number_integer())
    {
        read_latency_ = std::chrono::milliseconds(settings_json["read_latency"].get<int>());
    }
    else
    {
        read_latency_ = std::chrono::milliseconds(0);
    }

    if (settings_json.contains("write_latency") && 
        !settings_json["write_latency"].is_null() &&
         settings_json["write_latency"].is_number_integer())
    {
        write_letency_ = std::chrono::milliseconds(settings_json["write_latency"].get<int>());
    }
    else
    {
        write_letency_ = std::chrono::milliseconds(0);
    }

    if (settings_json.contains("move_latency") && 
        !settings_json["move_latency"].is_null() &&
         settings_json["move_latency"].is_number_integer())
    {
        move_latency_ = std::chrono::milliseconds(settings_json["move_latency"].get<int>());
    }
    else
    {
        move_latency_ = std::chrono::milliseconds(0);
    }
}

void TapeDevice::MoveMagnetHeadLeft()
{
    // Not implemented. Not nessesary for current iteration and implementing External sort algorithm
    MoveLatency(1);
}
void TapeDevice::RewindMagnetHead(streampos pos) // move seekp to std::ios::end in this stage of implementation
{
    if (pos == 0)
    {
        // Move to the begining. This used only when algorithm needs to rewrite all data in the tape
        // that's why in this implementation TapeDevice will erase all data from file
        // to avoid aliasing of new and old data in file
        std::cout << "RewindMagnetHead" << std::endl;
        file_.close();
        file_.open(path_, std::ios::in | std::ios::out | std::ios::trunc);
        MoveLatency(magnet_head_pos_);
        // state_ = State::normal;
        magnet_head_pos_ = 0;
    }
    // Not implemented. Not nessesary for current iteration and implementing External sort algorithm
}

} // namespace io