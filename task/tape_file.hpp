#pragma once

// std
#include <cstddef>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <chrono>

// nlohmann
#include <nlohmann/json.hpp>

//
#include <tape_device.hpp>

namespace io {


class File 
{
public:
    using buffer_type = std::vector<int>;
    using size_type = std::size_t;
public:
    File() = default;
    virtual ~File() { Close(); };

    // virtual void Open(std::ios::openmode mode = std::ios::in | std::ios::out) = 0;
    // virtual void Open(std::filesystem::path, std::ios::openmode mode = std::ios::in | std::ios::out) = 0;
    virtual void Read(buffer_type& dst, size_type size) = 0;
    virtual void Write(buffer_type& src, size_type size) = 0;
    virtual void Close(){};

    // In this implementation functions tellg, seekp will not be implemented due to deadlines
    virtual void Seekp(size_type pos) = 0; // return value void or should be *this as in the STL
    virtual size_type Tellp() = 0;

    virtual bool Eof() = 0;
    // virtual bool IsOpen() = 0;
};

class TapeFile : public File
{
public:
    TapeFile(std::filesystem::path tape_path, const nlohmann::json& device_settings);
    
    // virtual void Open(std::ios::openmode mode = std::ios::in | std::ios::out);
    // virtual void Open(std::filesystem::path, std::ios::openmode mode = std::ios::in | std::ios::out);
    virtual void Read(buffer_type& dst, size_type size) override;
    virtual void Write(buffer_type& src, size_type size) override;
    virtual void Close() override { device_.Close(); };

    virtual void Seekp(size_type pos) override; // return value void or should be *this as in the STL
    virtual size_type Tellp() override;

    // eof
    virtual bool Eof() { return device_.EndOfLine(); }


private:
    TapeDevice device_;
};

};