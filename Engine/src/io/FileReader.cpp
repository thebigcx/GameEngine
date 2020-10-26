#include "io/FileReader.h"

std::string FileReader::readFile(const std::string& path)
{
    std::ifstream file(path);

    std::ostringstream stream;
    stream << file.rdbuf();
    return stream.str();
}