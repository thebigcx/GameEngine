#include "FileSelectWindow.h"

#include <filesystem>

#include <imgui/imgui.h>

#include <maths/random.h>

std::filesystem::path FileSelectWindow::m_workingPath = std::filesystem::current_path();
std::filesystem::path FileSelectWindow::m_selection;
bool FileSelectWindow::m_madeSelection = false;
bool FileSelectWindow::m_isOpen = false;
std::string FileSelectWindow::m_title = "";
std::vector<std::string> FileSelectWindow::m_acceptedFileTypes;
int FileSelectWindow::m_acceptedFileTypeSelected;
std::string FileSelectWindow::m_searchQuery = "";

bool FileSelectWindow::display()
{
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse;
    bool open = true;

    ImGui::OpenPopup(m_title.c_str());

    if (ImGui::BeginPopupModal(m_title.c_str(), &open, flags))
    {
        ImGui::Text("Search:");
        ImGui::SameLine();

        char buf[128];

        strcpy(buf, m_searchQuery.c_str());
        ImGui::InputText("", buf, 128);
        m_searchQuery = buf;
        
        ImGui::BeginChild("", ImVec2{ImGui::GetContentRegionAvailWidth(), ImGui::GetWindowSize().y - 130});

        recurseTree(m_workingPath, 0);

        ImGui::EndChild();

        ImGui::Text("File:");
        ImGui::SameLine();

        memset(buf, 0, 128);
        strcpy(buf, m_selection.c_str());
        ImGuiInputTextFlags flags = ImGuiInputTextFlags_ReadOnly;
        ImGui::InputText("File Name:", buf, 128, flags);

        ImGui::SameLine();

        const char* current;

        if (m_acceptedFileTypes.size() == 0)
        {
            current = "<any>";
        }
        else if (m_acceptedFileTypeSelected >= m_acceptedFileTypes.size())
        {
            current = "";
        }
        else
        {
            current = m_acceptedFileTypes[m_acceptedFileTypeSelected].c_str();
        }

        ImGui::SetNextItemWidth(100);
        if (ImGui::BeginCombo("AcceptedFileTypes", current))
        {
            for (unsigned int i = 0; i < m_acceptedFileTypes.size(); i++)
            {
                bool isSelected = current == m_acceptedFileTypes[i];
                if (ImGui::Selectable(m_acceptedFileTypes[i].c_str(), isSelected))
                {
                    m_acceptedFileTypeSelected = i;
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
            m_madeSelection = false;
            m_isOpen = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Ok"))
        {
            m_madeSelection = true;
            m_isOpen = false;
        }

        ImGui::EndPopup();
    }
    

    return m_isOpen;
}

void FileSelectWindow::recurseTree(const std::filesystem::path& path, int level)
{
    if (ImGui::Selectable("[Dir] .."))
    {
        m_workingPath = m_workingPath.parent_path();
    }

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
            if (m_searchQuery != "")
            {
                if (std::string(entry.path().filename()).substr(0, m_searchQuery.size()) == m_searchQuery)
                {
                    if (m_acceptedFileTypes.size() == 0)
                    {
                        files.push_back(entry);
                    }
                    else if (m_acceptedFileTypes[m_acceptedFileTypeSelected] == entry.path().extension())
                    {
                        files.push_back(entry);
                    }
                }
            }
            else
            {
                if (m_acceptedFileTypes.size() == 0)
                {
                    files.push_back(entry);
                }
                else if (m_acceptedFileTypes[m_acceptedFileTypeSelected] == entry.path().extension())
                {
                    files.push_back(entry);
                }
            }
        }
    }

    for (auto& dir : directories)
    {
        std::string name = std::string("[Dir] ") + dir.path().filename().c_str();
        if (ImGui::Selectable(name.c_str()))
        {
            m_workingPath = dir.path();
        }
    }

    for (auto& file : files)
    {
        std::string name = std::string("[File] ") + file.path().filename().c_str();
        if (ImGui::Selectable(name.c_str()))
        {
            m_selection = std::filesystem::relative(file);
        }
    }
}

std::string FileSelectWindow::getSelection()
{
    return std::string(m_selection.c_str());
}