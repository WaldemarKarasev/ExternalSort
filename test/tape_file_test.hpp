#pragma once

// std
#include <vector>

// tape include
#include <tape_file.hpp>

namespace io::test {

// current implementation of the TapeFile will be used for the unit test of the External sort algorithm
// For this class TimeLatency simulation is unnecessary.
class TapeFileTest : public io::File
{
    using value_type = io::File::buffer_type::value_type;

public:

    TapeFileTest(std::vector<value_type> tape) 
        : File()
        , tape_(std::move(tape))
        , is_open_{true}
    {}

    virtual void Read(buffer_type& dst, size_type size) override 
    {
        if (magnet_head_pos_ + size > tape_.size())
        {
            size = tape_.size() - magnet_head_pos_;
        }

        dst.clear();
        for (; magnet_head_pos_ < size; ++magnet_head_pos_)
        {
            dst.push_back(tape_[magnet_head_pos_]);
        }

    }

    virtual void Write(buffer_type& src, size_type size) 
    {
        size_type emplace_steps = 0;
        size_type src_ptr = 0;
        if (magnet_head_pos_ + size > tape_.size())
        {
            emplace_steps = tape_.size() - magnet_head_pos_;
        }

        for (; magnet_head_pos_ < tape_.size(); ++magnet_head_pos_, ++src_ptr)
        {
            tape_[magnet_head_pos_] = src[src_ptr];
        }

        for (; src_ptr < size; ++src_ptr, ++magnet_head_pos_)
        {
            tape_.push_back(src[src_ptr]);
        }

    }
    virtual void Close() override { magnet_head_pos_ = 0; is_open_ = false; };

    virtual void Seekp(size_type pos) override 
    {
        if (pos > tape_.size() - 1)
        {
            pos = tape_.size() - 1;
        }

        magnet_head_pos_ = pos;
    }

    virtual size_type Tellp() override
    {
        return magnet_head_pos_;
    }

    virtual bool IsOpen() { return true; }


    virtual bool Eof() override
    {
        return magnet_head_pos_ == tape_.size() - 1;
    }

private:
    std::vector<value_type> tape_;
    std::size_t magnet_head_pos_{0};
    bool is_open_;
};

} // namespace io::test