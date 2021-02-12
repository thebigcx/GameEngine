#include "FileDialog.h"

#include <filesystem>
#include <algorithm>

#include <imgui/imgui.h>

#include <maths/random.h>
#include <core/Logger.h>

namespace Engine
{

FileDialog::FileDialog()
    : m_workingPath(std::filesystem::current_path())
{

}

bool FileDialog::display()
{
    auto instance = getInstance();

    if (!instance->m_id)
    {
        instance->m_id = instance;
    }
    ImGui::PushID(instance->m_id);

    if (instance->m_folderIcon == nullptr)
    {
        //instance->m_folderIcon = Texture2D::create("Editor/assets/folder_icon.png");
        //instance->m_fileIcon = Texture2D::create("Editor/assets/file_icon.png");
        instance->m_folderIcon = Texture2D::create("Editor/assets/transparent.png");
        instance->m_fileIcon = Texture2D::create("Editor/assets/transparent.png");
    }

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse;
    bool open = true;

    ImGui::OpenPopup(instance->m_title.c_str());

    if (ImGui::BeginPopupModal(instance->m_title.c_str(), &open, flags))
    {
        ImGui::Text("Search:");
        ImGui::SameLine();

        char buf[128];

        strcpy(buf, instance->m_searchQuery.c_str());
        ImGui::InputText("", buf, 128);
        instance->m_searchQuery = buf;
        
        ImGui::BeginChild("##explorer", ImVec2{ImGui::GetContentRegionAvailWidth(), ImGui::GetWindowSize().y - 130});

        try
        {
            renderDirectory(instance->m_workingPath);
        }
        catch (std::filesystem::filesystem_error e)
        {
            std::cout << "[FILESYSTEM] " << e.what() << "\n";
        }

        ImGui::EndChild();

        ImGui::Text("File:");
        ImGui::SameLine();

        memset(buf, 0, 128);

        
        if (instance->m_type == FileDialogType::Select)
        {
            strcpy(buf, instance->m_selection.c_str());
            ImGuiInputTextFlags flags = ImGuiInputTextFlags_ReadOnly;
        }
        else if (instance->m_type == FileDialogType::Save)
        {
            strcpy(buf, instance->m_fileName.c_str());
        }
        ImGui::InputText("File Name:", buf, 128, flags);

        instance->m_fileName = std::string(buf);


        ImGui::SameLine();

        const char* current;

        if (instance->m_acceptedFileTypes.size() == 0)
        {
            current = "<any>";
        }
        else if (instance->m_acceptedFileTypeSelected >= instance->m_acceptedFileTypes.size())
        {
            current = "";
        }
        else
        {
            current = instance->m_acceptedFileTypes[instance->m_acceptedFileTypeSelected].c_str();
        }

        ImGui::SetNextItemWidth(100);
        if (ImGui::BeginCombo("##acceptedFileFormats", current))
        {
            for (unsigned int i = 0; i < instance->m_acceptedFileTypes.size(); i++)
            {
                bool isSelected = current == instance->m_acceptedFileTypes[i];
                if (ImGui::Selectable(instance->m_acceptedFileTypes[i].c_str(), isSelected))
                {
                    instance->m_acceptedFileTypeSelected = i;
                }

                if (isSelected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }

            ImGui::EndCombo();
        }
        
        if (ImGui::Button("Cancel"))
        {
            instance->m_madeSelection = false;
            instance->m_isOpen = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Ok"))
        {
            instance->m_madeSelection = true;
            instance->m_isOpen = false;
        }

        ImGui::EndPopup();
    }
    
    ImGui::PopID();

    return instance->m_isOpen;
}

void FileDialog::renderDirectory(const std::filesystem::path& path)
{
    auto instance = getInstance();

    bool parentOpened = ImGui::Selectable("##..");

    ImGui::SameLine(0, 0);
    if (instance->m_flags & static_cast<uint32_t>(Flags::NoIcons))
        ImGui::Text("[Dir]");
    else
        ImGui::Image(reinterpret_cast<void*>(instance->m_folderIcon->getId()), ImVec2{ 20, 20 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
    ImGui::SameLine();
    ImGui::Text("..");

    if (parentOpened)
    {
        instance->m_workingPath = instance->m_workingPath.parent_path();
    }

    // Directories and files should be seperated
    std::vector<std::filesystem::directory_entry> directories;
    std::vector<std::filesystem::directory_entry> files;

    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        if (std::filesystem::is_directory(entry.status()))
        {
            directories.push_back(entry);
        }
        else if (std::filesystem::is_regular_file(entry.status()))
        {
            // Check if file fits search query
            if (instance->m_searchQuery != "")
            {
                if (std::string(entry.path().filename()).substr(0, instance->m_searchQuery.size()) == instance->m_searchQuery)
                {
                    if (instance->m_acceptedFileTypes.size() == 0)
                    {
                        files.push_back(entry);
                    }
                    else if (instance->m_acceptedFileTypes[instance->m_acceptedFileTypeSelected] == entry.path().extension())
                    {
                        files.push_back(entry);
                    }
                }
            }
            else
            {
                if (instance->m_acceptedFileTypes.size() == 0)
                {
                    files.push_back(entry);
                }
                else if (instance->m_acceptedFileTypes[instance->m_acceptedFileTypeSelected] == entry.path().extension())
                {
                    files.push_back(entry);
                }
            }
        }
    }

    // Sort files and directories alphabetically
    std::sort(directories.begin(), directories.end(), [&](auto dir1, auto dir2)
    {
        return std::string(dir1.path().filename()) < std::string(dir2.path().filename());
    });

    std::sort(files.begin(), files.end(), [&](auto file1, auto file2)
    {
        return std::string(file1.path().filename()) < std::string(file2.path().filename());
    });

    for (auto& dir : directories)
    {
        std::string name = dir.path().filename().c_str();

        bool opened = ImGui::Selectable((std::string("##") + name).c_str());

        ImGui::SameLine(0, 0);
        if (instance->m_flags & static_cast<uint32_t>(Flags::NoIcons))
            ImGui::Text("[Dir]");
        else
            ImGui::Image(reinterpret_cast<void*>(instance->m_folderIcon->getId()), ImVec2{ 20, 20 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
        ImGui::SameLine();
        ImGui::Text(name.c_str());

        if (opened)
        {
            instance->m_workingPath = dir.path();
        }
    }

    for (auto& file : files)
    {
        std::string name = file.path().filename().c_str();

        bool opened = ImGui::Selectable((std::string("##") + name).c_str());

        ImGui::SameLine(0, 0);
        if (instance->m_flags & static_cast<uint32_t>(Flags::NoIcons))
            ImGui::Text("[File]");
        else
            ImGui::Image(reinterpret_cast<void*>(instance->m_fileIcon->getId()), ImVec2{ 20, 20 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
        ImGui::SameLine();
        ImGui::Text(name.c_str());

        if (opened)
        {
            instance->m_selection = std::filesystem::relative(file);
        }
    }
}

std::string FileDialog::getSelection()
{
    auto instance = getInstance();

    if (instance->m_type == FileDialogType::Select)
    {
        return std::string(instance->m_selection.c_str());
    }
    else if (instance->m_type == FileDialogType::Save)
    {
        return std::string(instance->m_workingPath.c_str());
    }
}

}