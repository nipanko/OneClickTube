#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>
#include <cstdlib> // For system() function
#include <filesystem> // For filesystem operations

namespace fs = std::filesystem; // Alias for std::filesystem

// Callback to handle GLFW errors
void glfw_error_callback(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

// Function to display a simple file browser
bool FileBrowser(const char* label, std::string& path, bool& showBrowser) {
    static std::string currentPath = fs::current_path().string();
    static std::string selectedPath;

    if (showBrowser) {
        ImGui::Begin("File Browser", &showBrowser, ImGuiWindowFlags_AlwaysAutoResize);

        // Display current directory
        ImGui::Text("Current Directory: %s", currentPath.c_str());

        // List directories and files
        for (const auto& entry : fs::directory_iterator(currentPath)) {
            const auto& path = entry.path();
            std::string name = path.filename().string();

            if (fs::is_directory(path)) {
                name = "[Dir] " + name;
            }

            if (ImGui::Selectable(name.c_str())) {
                if (fs::is_directory(path)) {
                    // Navigate into the selected directory
                    currentPath = path.string();
                }
                else {
                    // Select the file
                    selectedPath = path.string();
                }
            }
        }

        // Go up one directory
        if (ImGui::Button("Up")) {
            currentPath = fs::path(currentPath).parent_path().string();
        }

        // Confirm selection
        ImGui::Separator();
        if (ImGui::Button("Select")) {
            path = selectedPath.empty() ? currentPath : selectedPath;
            showBrowser = false;
            ImGui::End();
            return true;
        }

        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            showBrowser = false;
        }

        ImGui::End();
    }

    return false;
}

int main() {
    // Set up GLFW
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        return -1;
    }

    // Set OpenGL version and profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Get primary monitor resolution
    GLFWmonitor* primary = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primary);

    // Create window in the center of the screen
    int window_width = 600, window_height = 300;
    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "One Click Tube", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwSetWindowPos(window, (mode->width - window_width) / 2, (mode->height - window_height) / 2);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize OpenGL loader!" << std::endl;
        return -1;
    }

    // Set up IMGUI context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    // Initialize IMGUI for GLFW and OpenGL3
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    char youtube_url[512] = "";
    std::string download_path = fs::current_path().string();
    bool download_requested = false;
    bool show_file_browser = false;

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Poll and handle events
        glfwPollEvents();

        // Start the IMGUI frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Create IMGUI UI
        ImGui::Begin("YouTube Downloader");
        ImGui::Text("Paste YouTube Link Below:");
        ImGui::InputText("##YouTube URL", youtube_url, IM_ARRAYSIZE(youtube_url));

        ImGui::Text("Download Path:");
        ImGui::InputText("##Download Path", &download_path[0], download_path.size() + 1, ImGuiInputTextFlags_ReadOnly);
        ImGui::SameLine();
        if (ImGui::Button("Browse")) {
            show_file_browser = true;
        }

        if (ImGui::Button("Download")) {
            download_requested = true;
        }
        ImGui::End();

        // Handle file browser
        if (show_file_browser) {
            FileBrowser("File Browser", download_path, show_file_browser);
        }

        // Handle download request
        if (download_requested) {
            std::string command = "yt-dlp -f bestvideo+bestaudio --merge-output-format mp4 -o \"" + download_path + "/%(title)s.%(ext)s\" " + std::string(youtube_url);
            system(command.c_str());
            download_requested = false;
        }

        // Render
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.12f, 0.12f, 0.12f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap buffers
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}