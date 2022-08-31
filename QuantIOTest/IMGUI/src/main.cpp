#include <stdio.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"
#include "gui.hpp"

//MSC Compiler version number and compatibility
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

//Glfw callback error handling, In case a Glfw function fails, an error is reported to the Glfw error callback
static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int, char**) {

    //Setup window
    glfwSetErrorCallback(glfw_error_callback);

    //Initialize Glfw, returns True or False
    if (!glfwInit()) return 1;

    //GL 3.0 + GLSL 130 OpenGL Shading Language 
    const char* glsl_version = "#version 130";

    //Minimum OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    //Maximize window
    glfwWindowHint(GLFW_MAXIMIZED, GL_TRUE);

    //Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1080, 720, "QuantIO", NULL, NULL);

    //Check if window was initialized
    if (window == NULL) return 1;

    //OpenGL context to use the OpenGL API
    glfwMakeContextCurrent(window);

    //Enable vsync
    glfwSwapInterval(1);

    //Create ImGui context
    ImGui::CreateContext();

    //ImGui io
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags = ImGuiConfigFlags_NavEnableKeyboard       // Enable Keyboard Controls
        | ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigWindowsResizeFromEdges = false;

    //ImGui style options
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    style.Alpha = 1.0f;
    style.WindowBorderSize = 0.0f;
    style.FramePadding = ImVec2(style.FramePadding.x * 2.0f, style.FramePadding.y * 2.0f);
    style.FrameRounding = 6.0f;
    style.GrabRounding = 6.0f;
    style.GrabMinSize = 20.0f;
    style.FrameBorderSize = 0.0f;
    style.ItemSpacing = ImVec2(style.ItemSpacing.x * 2.0f, style.ItemSpacing.y * 2.0f);
    style.WindowPadding = ImVec2(style.WindowPadding.x, style.WindowPadding.y);
    style.WindowBorderSize = 0.0f;
    style.ColorButtonPosition = 0;
    QuantIO::StyleColorsCustom(&style);

    //Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    //Fonts Settings
    io.FontGlobalScale = 1.0f;
    //io.ConfigWindowsMoveFromTitleBarOnly = true;
    io.FontAllowUserScaling = false;
    io.ConfigDockingNoSplit = true;

    //Load fonts
    ImFont* segoeuiFont1 = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 26.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    ImFont* dejavuFont = io.Fonts->AddFontFromFileTTF("..\\..\\misc\\fonts\\DejaVuSans.ttf", 20.0f);
    io.FontDefault = dejavuFont == NULL ? segoeuiFont1 : dejavuFont;
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 16.0f);

    //Icons
    static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
    ImFontConfig icons_config; 
    icons_config.MergeMode = true; 
    icons_config.PixelSnapH = true;
    io.Fonts->AddFontFromFileTTF("..\\..\\misc\\fonts\\fa-solid-900.ttf", 15.0f, 
        &icons_config, icons_ranges);

    //io.FontDefault = faFontIcon;

    //Set clear color
    ImVec4 clear_color = ImVec4(0.137f, 0.137f, 0.137f, 1.0f); //ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    //Start state
    bool showMainWindow = true;

    //Populates all available window objects
    QuantIO::AddWindows();

    //Main loop
    while (!glfwWindowShouldClose(window))
    {
        //Poll and handle events (inputs, window resize, etc.)
        glfwPollEvents();

        //Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //Items that are shown by default
        QuantIO::MainWindow(&showMainWindow);
        QuantIO::MainMenuBar();
        QuantIO::MainStatusBar();

        //Close app
        if (QuantIO::closeApplication)  glfwSetWindowShouldClose(window, true);

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);

    //Glfw needs to be terminated once done
    glfwTerminate();

    return 0;
}

