#include "FileSelectWindow.h"

#include <filesystem>

#include <imgui/imgui.h>

#include <maths/random.h>

std::filesystem::path FileSelectWindow::m_workingPath = std::filesystem::current_path();
std::filesystem::path FileSelectWindow::m_selection;
bool FileSelectWindow::m_madeSelection = false;
bool FileSelectWindow::m_isOpen = false;

bool FileSelectWindow::display()
{
    ImGuiWindowFlags flags;

    bool windowStatus = true;

    ImGui::OpenPopup("FileSelect");
    
    ImGui::SetNextWindowSize(ImVec2{700, 500});
    if (ImGui::BeginPopupModal("FileSelect", &windowStatus, flags))
    {
        ImGui::Text("Path:");
        ImGui::SameLine();

        char buf[128];
        strcpy(buf, m_workingPath.c_str());
        ImGui::InputText("", buf, 128);
        if (std::filesystem::exists(std::filesystem::path(buf)))
        {
            m_workingPath = buf;
        }
        
        ImGui::BeginChild("");

        recurseTree(m_workingPath, 0);

        ImGui::EndChild();

        ImGui::EndPopup();
    }

    if (!windowStatus)
    {
        m_isOpen = false;
    }

    return m_isOpen;
}

void FileSelectWindow::recurseTree(const std::filesystem::path& path, int level)
{
    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        auto filename = entry.path().filename();
        ImGui::Separator();
        if (std::filesystem::is_directory(entry.status()))
        {
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth;
            if (ImGui::TreeNodeEx(filename.c_str(), flags))
            {
                recurseTree(entry, level + 1);
                ImGui::TreePop();
            }
        }
        else if (std::filesystem::is_regular_file(entry.status()))
        {
            if (ImGui::Selectable(filename.c_str()))
            {
                m_selection = std::filesystem::relative(entry);
                m_isOpen = false;
                m_madeSelection = true;
            }
        }
        else
        {
            
        }
    }
}

std::string FileSelectWindow::getSelection()
{
    return std::string(m_selection.c_str());
}