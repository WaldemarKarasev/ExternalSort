#include <tape_file.hpp>

#include <iostream>
#include <thread>

namespace io {

TapeFile::TapeFile(std::filesystem::path tape_path, std::ios::openmode mode, const nlohmann::json& settings_json)
    : File()
    , device_(std::move(tape_path), settings_json)
{
    device_.Open(mode);
}

void TapeFile::Read(buffer_type& dst, size_type size)
{     
    dst.clear(); // clear destination buffer
    for (size_type i = 0; i < size; ++i)
    {
        auto cell = device_.ReadCurrentCell();
        if (cell.has_value())
        {
            dst.push_back(cell.value());
        }
        device_.MoveMagnetHeadRightRead();
        if (device_.EndOfLine())
        {
            break;
        }
    }      
}

void TapeFile::Write(buffer_type& src, size_type size)
{
    // std::cout << "TapeFile::Write()" << std::endl;
    for (size_type i = 0; i < size; ++i)
    {
        device_.WriteCurrentCell(src[i]);
        device_.MoveMagnetHeadRightWrite();
    }
}

TapeFile::size_type TapeFile::Tellp()
{
    return device_.GetMagnetHeadPosition();
}

void TapeFile::Seekp(size_type pos)
{
    device_.RewindMagnetHead(pos);
}

} // namespace io