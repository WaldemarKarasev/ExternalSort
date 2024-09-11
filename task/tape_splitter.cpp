#include <tape_splitter.hpp>
#include <settings.hpp>


namespace service {

TapeSplitter::TapeSplitter(std::filesystem::path& input_file, std::size_t ram_percentage)
    : ram_percentage_(ram_percentage)
    , input_file_path_(input_file)
{}

std::tuple<std::filesystem::path, std::size_t> TapeSplitter::Split()
{
    std::cout << "----------------SPLIT START-----------------" << std::endl;
    std::cout << "----------------SPLIT GETSIZE-----------------" << std::endl;
    input_tape_file_size_ = GetInputTapeSize();
    std::cout << "----------------SPLIT SPLITTING-----------------" << std::endl;

    std::cout << input_tape_file_size_ << std::endl;
    std::size_t available_ram = input_tape_file_size_ * ram_percentage_ / 100;
    
    std::filesystem::path tmp_tape_path_ = std::filesystem::current_path() / "tapes/";

    if (!std::filesystem::exists(tmp_tape_path_))
    {
        if (!std::filesystem::create_directory(tmp_tape_path_))
        {
            std::cout << "error creating temporary directory for temporary tapes" << std::endl;
            return {};
        }
    }
    
    std::cout << "available_ram=" << available_ram << std::endl;
    io::TapeFile input_tape(input_file_path_);
    std::cout << "start batching input file" << std::endl;
    std::size_t tmp_file_counter = 0;
    while (!input_tape.Eof())
    {
        io::TapeFile::buffer_type batch_buf;
        input_tape.Read(batch_buf, available_ram);
        std::cout << "input_tape.Eof()-" << input_tape.Eof() << "; input_tape.IsOpen()-" << input_tape.IsOpen() << std::endl;
        for (auto elem : batch_buf)
        {
            std::cout << elem << " ";
        }
        std::cout << std::endl;


        std::filesystem::path tmp_file_path = tmp_tape_path_ / std::filesystem::path("file" + std::to_string(tmp_file_counter) + ".tape");

        ++tmp_file_counter;
        io::TapeFile tmp_file(tmp_file_path, std::ios::out);
        tmp_file.Write(batch_buf, batch_buf.size());
    }


    std::cout << "----------------SPLIT END-----------------" << std::endl;


    return {tmp_tape_path_, available_ram};
}

std::size_t TapeSplitter::GetInputTapeSize()
{
    io::TapeFile input_tape(input_file_path_);
    std::size_t tape_size = 0;
    if (input_tape.IsOpen())
    {
        io::TapeFile::buffer_type tmp_buf;

        while (!input_tape.Eof())
        {
            input_tape.Read(tmp_buf, 1);
            std::cout << "ASDASDAS" << std::endl;
            std::cout << tmp_buf.empty() << std::endl;
            
            ++tape_size;
        }
    }
    else 
    {
        std::cout << "failed to open file" << std::endl;
    }

    return tape_size;
}

} // namespace service