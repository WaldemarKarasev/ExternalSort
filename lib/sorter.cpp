// std
#include <algorithm>
#include <filesystem>
#include <vector>
#include <queue>

// tape include
#include <settings.hpp>
#include <sorter.hpp>
#include <tape_file.hpp>

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
        io::TapeFile temp_tape(path);
        io::TapeFile::buffer_type buffer;
        temp_tape.Read(buffer, batch_size_);

        std::sort(buffer.begin(), buffer.end());
        
        temp_tape.Seekp(0); // move magnet head to the start of the tape
        temp_tape.Write(buffer, buffer.size());
    }

    // external merge
    Merge();
    return {};
}

void Sorter::Merge()
{
    std::vector<io::TapeFile> files_to_merge;
    std::priority_queue<HeapNode, std::vector<HeapNode>, std::greater<HeapNode>> priority_heap;


    for (auto& path : std::filesystem::directory_iterator(temp_tapes_dir_))
    {
        files_to_merge.push_back(io::TapeFile(path));
    }

    for (size_t i = 0; i < files_to_merge.size(); ++i)
    {
        if (!files_to_merge[i].Eof())
        {
            io::TapeFile::buffer_type tmp_vec;
            files_to_merge[i].Read(tmp_vec, 1);

            if (!tmp_vec.empty())
            {
                HeapNode node = {tmp_vec.front(), i};
                // std::cout << node.value_ << " " << node.index_ << std::endl; 
                priority_heap.push(node);
            }
        }
    }

    if (output_path_.empty())
    {
        output_path_ = std::filesystem::current_path() / "result.tape"; // just in case ??? 
    }
    io::TapeFile output_sorted_tape(output_path_, std::ios::out | std::ios::trunc);
    while (!priority_heap.empty())
    {
        HeapNode node = priority_heap.top();
        priority_heap.pop();
        auto value = node.value_;
        auto index = node.index_;

        {
            io::TapeFile::buffer_type tmp_vec{value};
            output_sorted_tape.Write(tmp_vec, 1);
        }

        if (!files_to_merge[index].Eof())
        {
            io::TapeFile::buffer_type tmp_vec;
            files_to_merge[index].Read(tmp_vec, 1);
            HeapNode read_node{tmp_vec.front(), index};
            priority_heap.push(read_node);
        }
    }
}

} // namespace algo