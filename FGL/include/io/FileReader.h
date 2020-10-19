#pragma once

#include <string>
#include <sstream>
#include <fstream>

class FileReader
{
public:
    static std::string readFile(const std::string& path);
};