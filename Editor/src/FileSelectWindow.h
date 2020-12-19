#pragma once

#include <string>
#include <vector>
#include <filesystem>

#include <core/Core.h>
#include <renderer/Texture2D.h>

class FileSelectWindow
{
public:
    static bool display();

    static void recurseTree(const std::filesystem::path& path, int level);

    static std::string getSelection();

    template<typename... Args>
    static bool selectFile(const std::string& title, Args... args)
    {
        m_title = title;

        m_acceptedFileTypes = { args... };

        return m_isOpen;
    }

    static bool madeSelection()
    {
        return m_madeSelection;
    }

    static void open()
    {
        // Reset states
        m_madeSelection = false;
        m_selection = "";
        m_isOpen = true;
    }

private:
    static std::filesystem::path m_workingPath;
    static std::filesystem::path m_selection;

    static bool m_madeSelection;
    static bool m_isOpen;

    static std::string m_title;

    static std::vector<std::string> m_acceptedFileTypes;
    static int m_acceptedFileTypeSelected;

    static std::string m_searchQuery;
};