#pragma once

#include <string>
#include <vector>
#include <filesystem>

#include <core/Core.h>
#include <renderer/Texture2D.h>

enum class FileDialogType
{
    Save,
    Select
};

class FileSelectWindow // TODO: separate select/save file dialogs into inherited classes of base class "FileDialog"
{
private:
    FileSelectWindow();
    ~FileSelectWindow() = default;

public:
    static bool display();

    static void renderDirectory(const std::filesystem::path& path);

    static std::string getSelection();

    template<typename... Args>
    static bool selectFile(const void* id, const std::string& title = "Choose file", Args... args)
    {
        if (m_instance.m_id != id)
        {
            return false;
        }
        m_instance.m_title = title;
        m_instance.m_acceptedFileTypes = { args... };
        m_instance.m_type = FileDialogType::Select;

        return m_instance.m_isOpen;
    }

    template<typename... Args>
    static bool saveFile(const void* id, const std::string& title = "Save file", Args... args)
    {
        if (m_instance.m_id != id)
        {
            return false;
        }
        m_instance.m_title = title;
        m_instance.m_acceptedFileTypes = { args... };
        m_instance.m_type = FileDialogType::Save;

        return m_instance.m_isOpen;
    }

    static bool madeSelection()
    {
        return m_instance.m_madeSelection;
    }

    static void open(const void* id)
    {
        // Reset states
        m_instance.m_id = id;

        m_instance.m_madeSelection = false;
        m_instance.m_selection = "";
        m_instance.m_isOpen = true;
        m_instance.m_workingPath = std::filesystem::current_path();
    }

    static std::string getFilename()
    {
        return "filename";
    }

    static std::string getSaveFileName()
    {
        return m_instance.m_fileName;
    }

private:
    static FileSelectWindow m_instance;

private:
    std::filesystem::path m_workingPath;
    std::filesystem::path m_selection;

    bool m_madeSelection;
    bool m_isOpen;

    std::string m_title;

    std::vector<std::string> m_acceptedFileTypes;
    int m_acceptedFileTypeSelected;

    std::string m_searchQuery;

    Shared<Texture2D> m_folderIcon;
    Shared<Texture2D> m_fileIcon;

    const void* m_id = 0;

    FileDialogType m_type;
    std::string m_fileName;
};