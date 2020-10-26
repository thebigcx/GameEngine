#pragma once

#include <string>
#include <fstream>

class FileWriter
{
public:
    static void writeToFile(const std::string& path, const std::string& text);
};