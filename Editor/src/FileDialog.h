#pragma once

#include <string>
#include <vector>
#include <filesystem>

#include <core/Core.h>
#include <renderer/Texture2D.h>

namespace Engine
{

enum class FileDialogType
{
    Save,
    Select
};

class FileDialog // TODO: separate select/save file dialogs into inherited classes of base class "FileDialog"
{
private:
    FileDialog();
    ~FileDialog()
    {

    }

public:
    enum class Flags
    {
        NoIcons
    };

public:
    static bool display();

    static void renderDirectory(const std::filesystem::path& path);

    static std::string getSelection();

    template<typename... Args>
    static bool selectFile(const void* id, const std::string& title = "Choose file", Args... args)
    {
        auto instance = getInstance();

        instance->m_flags = 0;
        if (instance->m_id != id)
        {
            return false;
        }
        instance->m_title = title;
        instance->m_acceptedFileTypes = { args... };
        instance->m_type = FileDialogType::Select;

        return instance->m_isOpen;
    }

    template<typename... Args>
    static bool selectFile(const void* id, uint32_t flags, const std::string& title = "Choose file", Args... args)
    {
        auto instance = getInstance();

        instance->m_flags = flags;
        if (instance->m_id != id)
        {
            return false;
        }
        instance->m_title = title;
        instance->m_acceptedFileTypes = { args... };
        instance->m_type = FileDialogType::Select;

        return instance->m_isOpen;
    }

    template<typename... Args>
    static bool saveFile(const void* id, const std::string& title = "Save file", Args... args)
    {
        auto instance = getInstance();

        if (instance->m_id != id)
        {
            return false;
        }
        instance->m_title = title;
        instance->m_acceptedFileTypes = { args... };
        instance->m_type = FileDialogType::Save;

        return instance->m_isOpen;
    }

    static bool madeSelection()
    {
        return getInstance()->m_madeSelection;
    }

    static void open(const void* id)
    {
        auto instance = getInstance();

        // Reset states
        instance->m_id = id;

        instance->m_madeSelection = false;
        instance->m_selection = "";
        instance->m_isOpen = true;
        instance->m_workingPath = std::filesystem::current_path();
        instance->m_flags = 0;
    }

    static std::string getFilename()
    {
        return "filename";
    }

    static std::string getSaveFileName()
    {
        return getInstance()->m_fileName;
    }

    static FileDialog* getInstance()
    {
        static FileDialog dialog;
        return &dialog;
    }

private:
    std::filesystem::path m_workingPath;
    std::filesystem::path m_selection;

    bool m_madeSelection;
    bool m_isOpen;

    std::string m_title;

    std::vector<std::string> m_acceptedFileTypes;
    int m_acceptedFileTypeSelected;

    std::string m_searchQuery;

    Reference<Texture2D> m_folderIcon;
    Reference<Texture2D> m_fileIcon;

    uint32_t m_flags;

    const void* m_id = 0;

    FileDialogType m_type;
    std::string m_fileName;
};

}