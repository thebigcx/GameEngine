#include "FileSelectWindow.h"

#include <filesystem>
#include <algorithm>

#include <imgui/imgui.h>

#include <maths/random.h>
#include <core/Logger.h>

FileSelectWindow FileSelectWindow::m_instance;

FileSelectWindow::FileSelectWindow()
    : m_workingPath(std::filesystem::current_path())
{

}

bool FileSelectWindow::display()
{
    if (!m_instance.m_id)
    {
        m_instance.m_id = &m_instance;
    }
    ImGui::PushID(m_instance.m_id);

    if (m_instance.m_folderIcon == nullptr)
    {
        m_instance.m_folderIcon = Texture2D::create("Editor/assets/folder_icon.png");
        m_instance.m_fileIcon = Texture2D::create("Editor/assets/file_icon.png");
    }

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse;
    bool open = true;

    ImGui::OpenPopup(m_instance.m_title.c_str());

    if (ImGui::BeginPopupModal(m_instance.m_title.c_str(), &open, flags))
    {
        ImGui::Text("Search:");
        ImGui::SameLine();

        char buf[128];

        strcpy(buf, m_instance.m_searchQuery.c_str());
        ImGui::InputText("", buf, 128);
        m_instance.m_searchQuery = buf;
        
        ImGui::BeginChild("##explorer", ImVec2{ImGui::GetContentRegionAvailWidth(), ImGui::GetWindowSize().y - 130});

        try
        {
            renderDirectory(m_instance.m_workingPath);
        }
        catch (std::filesystem::filesystem_error e)
        {
            std::cout << "[FILESYSTEM] " << e.what() << "\n";
        }

        ImGui::EndChild();

        ImGui::Text("File:");
        ImGui::SameLine();

        memset(buf, 0, 128);
        strcpy(buf, m_instance.m_selection.c_str());
        ImGuiInputTextFlags flags = ImGuiInputTextFlags_ReadOnly;
        ImGui::InputText("File Name:", buf, 128, flags);

        ImGui::SameLine();

        const char* current;

        if (m_instance.m_acceptedFileTypes.size() == 0)
        {
            current = "<any>";
        }
        else if (m_instance.m_acceptedFileTypeSelected >= m_instance.m_acceptedFileTypes.size())
        {
            current = "";
        }
        else
        {
            current = m_instance.m_acceptedFileTypes[m_instance.m_acceptedFileTypeSelected].c_str();
        }

        ImGui::SetNextItemWidth(100);
        if (ImGui::BeginCombo("##acceptedFileFormats", current))
        {
            for (unsigned int i = 0; i < m_instance.m_acceptedFileTypes.size(); i++)
            {
                bool isSelected = current == m_instance.m_acceptedFileTypes[i];
                if (ImGui::Selectable(m_instance.m_acceptedFileTypes[i].c_str(), isSelected))
                {
                    m_instance.m_acceptedFileTypeSelected = i;
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
            m_instance.m_madeSelection = false;
            m_instance.m_isOpen = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Ok"))
        {
            m_instance.m_madeSelection = true;
            m_instance.m_isOpen = false;
        }

        ImGui::EndPopup();
    }
    
    ImGui::PopID();

    return m_instance.m_isOpen;
}

void FileSelectWindow::renderDirectory(const std::filesystem::path& path)
{
    bool parentOpened = ImGui::Selectable("##..");

    ImGui::SameLine(0, 0);
    ImGui::Image(reinterpret_cast<void*>(m_instance.m_folderIcon->getId()), ImVec2{ 20, 20 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
    ImGui::SameLine();
    ImGui::Text("..");

    if (parentOpened)
    {
        m_instance.m_workingPath = m_instance.m_workingPath.parent_path();
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
            if (m_instance.m_searchQuery != "")
            {
                if (std::string(entry.path().filename()).substr(0, m_instance.m_searchQuery.size()) == m_instance.m_searchQuery)
                {
                    if (m_instance.m_acceptedFileTypes.size() == 0)
                    {
                        files.push_back(entry);
                    }
                    else if (m_instance.m_acceptedFileTypes[m_instance.m_acceptedFileTypeSelected] == entry.path().extension())
                    {
                        files.push_back(entry);
                    }
                }
            }
            else
            {
                if (m_instance.m_acceptedFileTypes.size() == 0)
                {
                    files.push_back(entry);
                }
                else if (m_instance.m_acceptedFileTypes[m_instance.m_acceptedFileTypeSelected] == entry.path().extension())
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
        ImGui::Image(reinterpret_cast<void*>(m_instance.m_folderIcon->getId()), ImVec2{ 20, 20 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
        ImGui::SameLine();
        ImGui::Text(name.c_str());

        if (opened)
        {
            m_instance.m_workingPath = dir.path();
        }
    }

    for (auto& file : files)
    {
        std::string name = file.path().filename().c_str();

        bool opened = ImGui::Selectable((std::string("##") + name).c_str());

        ImGui::SameLine(0, 0);
        ImGui::Image(reinterpret_cast<void*>(m_instance.m_fileIcon->getId()), ImVec2{ 20, 20 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
        ImGui::SameLine();
        ImGui::Text(name.c_str());

        if (opened)
        {
            m_instance.m_selection = std::filesystem::relative(file);
        }
    }
}

std::string FileSelectWindow::getSelection()
{
    return std::string(m_instance.m_selection.c_str());
}