#include <util/io/FileSystem.h>

#include <filesystem>

namespace Engine
{

std::string FileSystem::m_assetDirectoryPath = "./";

FileStream::FileStream(FILE* file, Mode mode)
    : m_file(file), m_mode(mode)
{

}

FileStream::~FileStream()
{
    fclose(m_file);
}

Shared<FileStream> FileStream::create(const std::string& filepath, Mode mode)
{
    FILE* file;

    if (mode == FileStream::Mode::Read)
        file = fopen(filepath.c_str(), "r");
    else if (mode == FileStream::Mode::Write)
        file = fopen(filepath.c_str(), "w");
        
    return Shared<FileStream>(new FileStream(file, mode));
}

void FileSystem::setAssetDirectoryPath(const std::string& path)
{
    m_assetDirectoryPath = path;
}

std::string FileSystem::getAssetPath(const std::string& asset)
{
    return m_assetDirectoryPath + asset;
}

std::string FileSystem::getCurrentDirectory()
{
    return static_cast<std::string>(std::filesystem::current_path()) + "/";
}

void FileSystem::setCurrentDirectory(const std::string& path)
{
    std::filesystem::current_path(path);
}

void FileSystem::writeToFile(const std::string& path, const std::string& text)
{
    std::ofstream file;
    file.open(path, std::ios::app);
    file << text;
    file.close();
}

std::string FileSystem::readFile(const std::string& path)
{
    std::ifstream file(path);

    std::ostringstream stream;
    stream << file.rdbuf();
    return stream.str();
}

void FileSystem::writeToBinaryFile(const std::string& path, void* data, size_t size)
{
    char* byteData = reinterpret_cast<char*>(data);

    std::ofstream file(path, std::ios::out | std::ios::binary);

    file.write(byteData, size);

    file.close();
}

char* FileSystem::readBinaryFile(const std::string& path, size_t size)
{
    std::ifstream file(path, std::ios::in | std::ios::binary);

    char* data = new char[size];

    file.read(&data[0], size);

    file.close();

    return data;
}

bool FileSystem::exists(const std::string& path)
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

std::string FileSystem::getExtension(const std::string& path)
{
    return path.substr(path.find_last_of(".") + 1);
}

}