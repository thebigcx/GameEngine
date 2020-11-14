#include <util/io/Files.h>

void Files::writeToFile(const std::string& path, const std::string& text)
{
    std::ofstream file;
    file.open(path, std::ios::app);
    file << text;
    file.close();
}

std::string Files::readFile(const std::string& path)
{
    std::ifstream file(path);

    std::ostringstream stream;
    stream << file.rdbuf();
    return stream.str();
}

void Files::writeToBinaryFile(const std::string& path, void* data)
{

}

void* Files::readBinaryFile(const std::string& path)
{

}

bool Files::exists(const std::string& path)
{
    FILE* file = fopen(path.c_str(), "r");

    if (file)
    {
        fclose(file);
        return true;
    }
    else
    {
        return false;
    }
}