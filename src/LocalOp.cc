#include "ClientTool.h"
uint64_t get_file_size(const string &filepath)
{
    ifstream inFile(filepath, std::ios::in);
    if (!inFile.is_open())
    {
        std::cout << "Open file " << filepath << " failed!" << std::endl;
        return -1;
    }
    inFile.seekg(0, std::ios_base::end);
    uint64_t fileSize = inFile.tellg();
    inFile.seekg(0, std::ios_base::beg);
    inFile.close();
    return fileSize;
}