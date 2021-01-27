#pragma once

#include <string>
#include <sstream>
#include <fstream>

#include <core/Core.h>

namespace Engine
{

class FileStream
{
public:
    enum class Mode
    {
        Read,
        Write
    };

public:
    static Shared<FileStream> create(const std::string& filepath, Mode mode);

private:
    FileStream(FILE* file, Mode mode);

    FILE* m_file;
    Mode m_mode;

public:
    ~FileStream();
};

class FileSystem
{
public:
    static void setAssetDirectoryPath(const std::string& path);
    static inline const std::string& getAssetDirectoryPath() { return m_assetDirectoryPath; }

    static void setCurrentDirectory(const std::string& path);
    static std::string getCurrentDirectory();

    static std::string getAssetPath(const std::string& asset);

    static void writeToFile(const std::string& path, const std::string& text);
    static std::string readFile(const std::string& path);

    static void writeToBinaryFile(const std::string& path, void* data, size_t size);
    static char* readBinaryFile(const std::string& path, size_t size);

    static bool exists(const std::string& path);

    static std::string getExtension(const std::string& path);

private:
    static std::string m_assetDirectoryPath;
};

}