#include <tape_splitter.hpp>
#include <settings.hpp>


namespace service {

TapeSplitter::TapeSplitter(std::filesystem::path& input_file, std::size_t ram_percentage)
    : ram_percentage_(ram_percentage)
    , input_file_path_(input_file)
{}

std::tuple<std::filesystem::path, std::size_t> TapeSplitter::Split()
{
    input_tape_file_size_ = GetInputTapeSize();

    std::size_t available_ram = input_tape_file_size_ * ram_percentage_ / 100;
    
    std::filesystem::path tmp_tape_path_ = "tapes";
    if (!std::filesystem::create_directories(tmp_tape_path_))
    {
        std::cout << "error creating temporary directory for temporary tapes" << std::endl;
        return {};
    }

    io::TapeFile input_tape(input_file_path_, settings::global_settings);
    std::cout << "start batching input file" << std::endl;
    std::size_t tmp_file_counter = 0;
    while (!input_tape.Eof())
    {
        io::TapeFile::buffer_type batch_buf;
        input_tape.Read(batch_buf, available_ram);
        
        for (auto elem : batch_buf)
        {
            std::cout << elem << " ";
        }
        std::cout << std::endl;
        
        std::filesystem::path tmp_file_path = tmp_tape_path_ / std::filesystem::path("file" + std::to_string(tmp_file_counter) + ".tape");
        io::TapeFile tmp_file(tmp_file_path, settings::global_settings);
        tmp_file.Write(batch_buf, batch_buf.size());
    }

    std::cout << "end batching input file" << std::endl;


    return {tmp_tape_path_, available_ram};
}

std::size_t TapeSplitter::GetInputTapeSize()
{
    io::TapeFile input_tape(input_file_path_, settings::global_settings);

    std::size_t tape_size = 0;
    io::TapeFile::buffer_type tmp_buf;
    while (input_tape.Eof())
    {
        input_tape.Read(tmp_buf, 1);
        ++tape_size;
    }

    return tape_size;
}

} // namespace service