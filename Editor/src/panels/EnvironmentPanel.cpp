#include "EnvironmentPanel.h"
#include "../FileDialog.h"

#include <imgui/imgui.h>

#include <renderer/Renderer3D.h>
#include <renderer/Renderer.h>

namespace Engine
{

EnvironmentPanel::EnvironmentPanel()
{

}

void EnvironmentPanel::onImGuiRender()
{
    ImGui::Begin("Environment");

    if (ImGui::Button("Load Environment Map"))
    {
        FileDialog::open("loadEnvironmentMap");
    }

    if (FileDialog::selectFile("loadEnvironmentMap", "Choose environment map", ".hdr"))
    {
        if (!FileDialog::display())
        {
            if (FileDialog::madeSelection())
            {
                Renderer3D::setEnvironment(EnvironmentMap::create(FileDialog::getSelection()));
            }
        }
    }

    ImGui::Columns(2);

    ImGui::Text("Skybox LOD");
    ImGui::NextColumn();
    //ImGui::SliderFloat("##skyboxLOD", &lod, 0, 10);
    ImGui::NextColumn();

    ImGui::Text("Exposure");
    ImGui::NextColumn();
    float exposure = Renderer::hdrExposure;
    ImGui::SliderFloat("##Exposure", &exposure, 0.f, 5.f);
    Renderer::hdrExposure = exposure;
    ImGui::NextColumn();

    ImGui::Columns(1);

    ImGui::End();
}

}