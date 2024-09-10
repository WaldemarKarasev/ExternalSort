#include <sorter.hpp>


// std
#include <algorithm>
#include <filesystem>
#include <vector>

// tape include
#include <tape_file.hpp>
#include <settings.hpp>

namespace algo {

std::filesystem::path Sorter::Sort() 
{
    // iterate and sort
    for (auto& path : std::filesystem::directory_iterator(temp_tapes_dir_))
    {
        io::TapeFile temp_tape(path, settings::global_settings);
        io::TapeFile::buffer_type buffer;
        temp_tape.Read(buffer, batch_size_);

        std::sort(buffer.begin(), buffer.end());

        temp_tape.Seekp(0); // move magnet head to the start of the tape

        temp_tape.Write(buffer, batch_size_);
    }

    // merge
    // Not implemented

}

} // namespace algo