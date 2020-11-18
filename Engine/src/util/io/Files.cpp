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

void Files::writeToBinaryFile(const std::string& path, void* data, size_t size)
{
    char* byteData = reinterpret_cast<char*>(data);

    std::ofstream file(path, std::ios::out | std::ios::binary);

    file.write(byteData, size);

    file.close();
}

char* Files::readBinaryFile(const std::string& path, size_t size)
{
    std::ifstream file(path, std::ios::in | std::ios::binary);

    char* data = new char[size];

    file.read(&data[0], size);

    file.close();

    return data;
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