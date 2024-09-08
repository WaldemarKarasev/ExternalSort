#include <io_device.hpp>

#include <iostream>
#include <thread>
#include <cmath>

namespace io {

TapeDevice::TapeDevice(std::filesystem::path path)
    : path_{std::move(path)}
{
}

void TapeDevice::Open()
{
    ReadFromFile();
}

void TapeDevice::Read(buffer_type& dst, size_type size)
{
    std::cout << "TapeDevice::Read()" << std::endl;

    size_type size_to_read = size;

    if (size_to_read > tape_buffer_.size())
    {
        size_to_read = tape_buffer_.size();
    }

    dst.clear(); // clear destination buffer
    for (size_type i = 0; i < size_to_read; ++i)
    {
        dst.push_back(tape_buffer_[i]);
        ReadLatency();
        magnet_head_pos_ += 1;
        MoveLatency(1);
    }
}

void TapeDevice::Write(buffer_type& src, size_type size)
{
    std::cout << "Write()" << std::endl;

    size_type size_to_write = src.size();

    if (size_to_write > (tape_buffer_.size() - magnet_head_pos_))
    {
        tape_buffer_.reserve(tape_buffer_.size() + (size_to_write - (tape_buffer_.size() - magnet_head_pos_)));
    }
    
    for (size_type i = magnet_head_pos_, j = 0; j < src.size(); ++i, ++j)
    {
        tape_buffer_[i] = src[j]; // back inserter
        WriteLatency();
        magnet_head_pos_ += 1;
        MoveLatency(1);
    }

    WriteToFile();
}

void TapeDevice::PrintFileState()
{
    std::cout << "p=" << file_.tellp() << "; good=" << file_.good() << "; eof=" << file_.eof() << "; fail=" << file_.fail()
    << "; bad=" << file_.bad() << "; rstate=" << file_.rdstate() << std::endl;
}

void TapeDevice::WriteToFile()
{
    file_.open(path_, std::ios::in | std::ios::trunc);
    PrintFileState();
    if (file_.is_open())
    {
        for (auto elem : tape_buffer_)
        {
            std::string num_str = std::to_string(elem) + "\n";
            file_.write(num_str.c_str(), num_str.size());
        }
    }
}

void TapeDevice::ReadFromFile()
{
    std::cout << "TapeDevice::ReadFromFile" << std::endl;
    char number[20]; // for int32 20 symbols are more than enough
    tape_buffer_.clear();
    file_.open(path_, std::ios::out | std::ios::in);
    if (file_.is_open())
    {
        while(file_.getline(number, 20))
        {   
            try
            {
                int num = std::stoi(number);
                std::cout << "number str=" << number << "; num=" << num << std::endl;
                tape_buffer_.push_back(num);
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
            if(file_.eof())
            {
                break;
            }
        }
    }
    else
    {
        std::cout << "fail" << std::endl;
    }
}

void TapeDevice::Close()
{
    file_.close();
}

void TapeDevice::Seekp(size_type pos)
{
    if (pos > tape_buffer_.size())
    {
        auto steps = tape_buffer_.size() - magnet_head_pos_;
        magnet_head_pos_ = tape_buffer_.size();
        MoveLatency(steps);
    }
    else
    {
        size_type steps;
        if (pos > magnet_head_pos_)
        {   
            steps = pos - magnet_head_pos_;
        }
        else
        {
            steps = magnet_head_pos_ - pos;
        }

        magnet_head_pos_ = pos;
        MoveLatency(steps);
    }
}

TapeDevice::size_type TapeDevice::Tellp()
{
    return magnet_head_pos_;
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

} // namespace io