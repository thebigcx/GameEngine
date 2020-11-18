#pragma once

#include <string>
#include <sstream>
#include <fstream>

class Files
{
public:
    static void writeToFile(const std::string& path, const std::string& text);
    static std::string readFile(const std::string& path);

    static void writeToBinaryFile(const std::string& path, void* data, size_t size);
    static char* readBinaryFile(const std::string& path, size_t size);

    static bool exists(const std::string& path);
};