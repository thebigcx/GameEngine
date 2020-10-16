#include "FGL/io/FileWriter.h"

void FileWriter::writeToFile(const std::string& path, const std::string& text)
{
    std::ofstream file;
    file.open(path, std::ios::app);
    file << text;
    file.close();
}