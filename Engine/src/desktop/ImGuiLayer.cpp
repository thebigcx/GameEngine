#include <desktop/ImGuiLayer.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imguizmo/ImGuizmo.h>

#include <core/Game.h>
#include <renderer/RenderCommand.h>

namespace Engine
{

ImGuiLayer::ImGuiLayer()
{

}

void ImGuiLayer::onAttach()
{
    ImGui::CreateContext();

    auto& io = ImGui::GetIO(); (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    io.FontDefault = io.Fonts->AddFontFromFileTTF("Editor/assets/Roboto.ttf", 18.f);

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 0.0f;
	style.Colors[ImGuiCol_WindowBg].w = 1.0f;

    auto& window = Game::getInstance().getWindow();

    ImGui_ImplGlfw_InitForOpenGL(window.getNative(), true);
	ImGui_ImplOpenGL3_Init("#version 460");
}

void ImGuiLayer::onUpdate(float dt)
{

}

void ImGuiLayer::begin()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();
}

void ImGuiLayer::end()
{
    auto& io = ImGui::GetIO();

    Game& app = Game::getInstance();
    io.DisplaySize = ImVec2((float)app.getWindow().getWidth(), (float)app.getWindow().getHeight());

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}

void ImGuiLayer::onDetach()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    ImGui::DestroyContext();
}

void ImGuiLayer::onEvent(Event& event)
{
    if (m_blockEvents)
    {
        auto& io = ImGui::GetIO();
        event.handled |= event.categories() & io.WantCaptureMouse;
        event.handled |= event.categories() & io.WantCaptureKeyboard;
    }
}

void ImGuiLayer::blockEvents(bool block)
{
    m_blockEvents = block;
}

}