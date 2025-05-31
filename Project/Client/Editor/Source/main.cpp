#include "CrashPCH.h"

#include "CrashEngine.h"
#include "CrashInputMgr.h"
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "CrashRenderProtocol.hpp"

#include "HelloCube02.h"

using namespace Crash;

const std::string   sTitle  = "CrashEditor";
int                 sWidth  = 1280;
int                 sHeight = 720;

GLFWwindow*         sWindow = nullptr;
Crash::Scene*       sScene  = nullptr;

std::pair<int, int> sGlVersion = { 3, 3 };

void processInput(GLFWwindow *window)
{
    //  Keyboard
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        InputMgr::Instance()->onKeyDown(InputMgr::KeyCode::KEY_W);
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        InputMgr::Instance()->onKeyDown(InputMgr::KeyCode::KEY_S);
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        InputMgr::Instance()->onKeyDown(InputMgr::KeyCode::KEY_A);
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        InputMgr::Instance()->onKeyDown(InputMgr::KeyCode::KEY_D);  
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) 
        InputMgr::Instance()->onKeyDown(InputMgr::KeyCode::MOUSE_BUTTON_RIGHT);  

    //  Mouse
    {
        static double lastX = sWidth / 2.0;
        static double lastY = sHeight / 2.0;
        static bool firstMouse = true;

        double xpos, ypos;
        glfwGetCursorPos(sWindow, &xpos, &ypos);

        double xoffset = 0.0, yoffset = 0.0;
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        } 
        else 
        {
            xoffset = xpos - lastX;
            yoffset = ypos - lastY;
            lastX = xpos;
            lastY = ypos;
        }
        InputMgr::Instance()->setMouseDelta(xoffset, yoffset);
    }
}

void renderMainEditor();

int main()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, sGlVersion.first);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, sGlVersion.second);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);   // For MacOS

    sWindow = glfwCreateWindow(sWidth, sHeight, sTitle.c_str(), nullptr, nullptr);
    if (!sWindow)    
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(sWindow);
    glfwMakeContextCurrent(nullptr);

    Crash::EngineConfig config = {};
    config.windowWidth  = sWidth;
    config.windowHeight = sHeight;
    config.outputFunc   = [](const std::string& log) { std::cout << log << std::endl; };
    config.procAddress  = reinterpret_cast<void*>(glfwGetProcAddress);
    
    config.asyncRender          = false;
    config.initRenderContext    = []() { glfwMakeContextCurrent(sWindow); };
    config.swapBuffer           = []() { glfwSwapBuffers(sWindow); };

    new Crash::Engine(config);
    sScene = new HelloCube02();
    Crash::Engine::Instance()->setScene(sScene);

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // 启用 Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // 启用多视口（多窗口）    
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(sWindow, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    auto start = std::chrono::steady_clock::now();
    auto now = std::chrono::steady_clock::now();
    double currentTime = std::chrono::duration<double>(now - start).count();
    double lastTime = currentTime;

    while (!glfwWindowShouldClose(sWindow))
    {
        processInput(sWindow);
        
        now = std::chrono::steady_clock::now();
        currentTime = std::chrono::duration<double>(now - start).count();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        Crash::Engine::Instance()->setExecuteTime(currentTime);
        Crash::Engine::Instance()->update(deltaTime); 

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        renderMainEditor();
        Crash::Engine::Instance()->renderFrame();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // 5. 处理多视口/平台窗口
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        Crash::Engine::Instance()->renderFinish();
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
    Crash::Engine::Instance()->setScene(nullptr);
    delete sScene;
    sScene = nullptr;
    delete Crash::Engine::Instance(); 

    glfwDestroyWindow(sWindow);
    glfwTerminate();

    return 0;
}

void renderMainEditor()
{
    ImGui::Begin("Main Client");

    //  Demo window
    {
        static bool show_demo_window = false;
        ImGui::Checkbox("Show Demo Window", &show_demo_window);
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);
    }

    ImGui::End();

    // {
    //     static int curFace = 0;
    //     const char* faceItems[]     = { "None", "Front", "Back", "FrontAndBack" };

    //     static int curPolygon = 0;
    //     const char* PolygonItems[]  = { "None", "Fill" , "Line" };
        
    //     ImGui::SetNextItemWidth(150);
    //     if(ImGui::Combo("FaceType", &curFace, faceItems, 4) && curFace && curPolygon)
    //     {
    //         Crash::Render::Instance()->setPolygonMode(
    //             (Crash::RenderProtocol::FaceMode)(curFace), 
    //             (Crash::RenderProtocol::PolygonMode)(curPolygon)
    //         );
    //     }

    //     ImGui::SameLine();  
        
    //     ImGui::SetNextItemWidth(150);
    //     if(ImGui::Combo("PolygonType", &curPolygon, PolygonItems, 3) && curFace && curPolygon)
    //     {
    //         Crash::Render::Instance()->setPolygonMode(
    //             (Crash::RenderProtocol::FaceMode)(curFace), 
    //             (Crash::RenderProtocol::PolygonMode)(curPolygon)
    //         );
    //     }
    // }
}