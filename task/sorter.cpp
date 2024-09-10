#include <sorter.hpp>


// std
#include <algorithm>
#include <filesystem>
#include <vector>

// tape include
#include <tape_file.hpp>
#include <settings.hpp>

namespace algo {

void Print(io::TapeFile::buffer_type& buffer)
{
    std::cout << std::endl;
    for(auto elem : buffer)
    {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}

std::filesystem::path Sorter::Sort() 
{
    // iterate and sort
    for (auto& path : std::filesystem::directory_iterator(temp_tapes_dir_))
    {
        std::cout << "AAAAAA - " << path << std::endl;
        io::TapeFile temp_tape(path);
        io::TapeFile::buffer_type buffer;
        temp_tape.Read(buffer, batch_size_);

        std::cout << "Printing buffer before sorting" << std::endl;
        Print(buffer);

        std::sort(buffer.begin(), buffer.end());
        std::cout << "Printing buffer after sorting" << std::endl;
        Print(buffer);
        temp_tape.Seekp(0); // move magnet head to the start of the tape
        temp_tape.Write(buffer, batch_size_);
    }

    // merge
    // Not implemented

}

} // namespace algo