#pragma once

#include <string>
#include <filesystem>

#include <core/Core.h>
#include <renderer/Texture2D.h>

class FileSelectWindow
{
public:
    static bool display();

    static void recurseTree(const std::filesystem::path& path, int level);

    static std::string getSelection();
    static bool isOpen()
    {
        return m_isOpen;
    }

    static bool madeSelection()
    {
        return m_madeSelection;
    }

    static void open()
    {
        m_isOpen = true;
    }

private:
    static std::filesystem::path m_workingPath;
    static std::filesystem::path m_selection;

    static bool m_madeSelection;
    static bool m_isOpen;
};