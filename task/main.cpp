#include <iostream>

#include <file.hpp>
#include <io_device.hpp>
#include <vector>
int main()
{
    // test
    io::TapeFile file("numbers.txt", std::ios::in | std::ios::out);
    
    io::TapeFile::buffer_type nums;

    file.Read(nums, 2);

    nums.push_back(7777);
    for (auto num : nums)
    {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    file.Write(nums, nums.size());

    // io::TapeFile::buffer_type nums2;
    // nums2.push_back(5555);
    // file.Write(nums2, nums2.size());


    // 1. Process input args

    // 2. Init IO_Device
    // 2.1 Read config file for IO_Device

    // 3. Batch source file into different batches in /tmp folder

    // 4. Sorting
    // 4.1 Sorting each batch
    // 4.2 External merge
    // 4.3 Output result filename
}