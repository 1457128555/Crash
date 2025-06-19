#include "CrashPCH.h"
#include "EditorRoot.h"

//  IMGUI
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

using namespace Crash;

EditorRoot::EditorRoot(void* wHnd, const std::string& glsl_version)
{
    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   //  Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; //  Viewports    
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)wHnd, true);
    ImGui_ImplOpenGL3_Init(glsl_version.c_str());
}

EditorRoot::~EditorRoot()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void EditorRoot::frameBegin()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void EditorRoot::frameEnd()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void EditorRoot::updatePlatformWindows()
{
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
}

void EditorRoot::render()
{
    ImGui::Begin("Editor Root");

    //  Demo window
    {
        static bool show_demo_window = false;
        ImGui::Checkbox("Show Demo Window", &show_demo_window);
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);
    }

    ImGui::End();
}

bool EditorRoot::checkViewportsEnable()
{
    return ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable;
}