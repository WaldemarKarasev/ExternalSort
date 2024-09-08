#include <file.hpp>

#include <iostream>
#include <thread>

namespace io {

TapeFile::TapeFile(std::filesystem::path tape_path, std::ios::openmode mode)
    : File()
    , device_(std::move(tape_path))
{
    device_.Open();
}

void TapeFile::Read(buffer_type& dst, size_type size)
{
    std::cout << "TapeFile::Read()" << std::endl;
    device_.Read(dst, size);
}

void TapeFile::Write(buffer_type& src, size_type size)
{
    device_.Write(src, size);
}

TapeFile::size_type TapeFile::Tellp()
{
    return device_.Tellp();
}

void TapeFile::Seekp(size_type pos)
{
    device_.Seekp(pos);
}

} // namespace io