#include "EditorLayer.h"

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include <core/Application.h>
#include <renderer/RenderCommand.h>

EditorLayer::EditorLayer()
{

}

void EditorLayer::onAttach()
{
    ImGui::CreateContext();

    auto& io = ImGui::GetIO(); (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    io.Fonts->AddFontFromFileTTF("Editor/assets/Roboto.ttf", 18.f);
    io.FontDefault = io.Fonts->AddFontFromFileTTF("Editor/assets/Roboto.ttf", 18.f);

    ImGui::StyleColorsDark();

    auto window = Application::get().getWindow().getNative();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 460");
}

void EditorLayer::onUpdate()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    bool show = true;
    ImGui::ShowDemoWindow(&show);

    ImGui::Begin("Hello, world!");
    ImGui::Text("This is a text.");
    ImGui::End();

    ImGui::EndFrame();

    ImGui::Render();
    RenderCommand::clear(RenderCommand::defaultClearBits());
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    auto& io = ImGui::GetIO();

    Application& app = Application::get();
    io.DisplaySize = ImVec2((float)app.getWindow().getWidth(), (float)app.getWindow().getHeight());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}

void EditorLayer::onDetach()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    ImGui::DestroyContext();
}



void EditorLayer::onEvent(Event& event)
{
    if (m_blockEvents)
    {
        auto& io = ImGui::GetIO();
        event.handled |= event.getCategoryFlags() & io.WantCaptureMouse;
        event.handled |= event.getCategoryFlags() & io.WantCaptureKeyboard;
    }
}