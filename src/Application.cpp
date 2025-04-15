#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "TestScene.h"

constexpr float MIN_FPS = 60;
constexpr float ALLOWED_LOW_FPS_FRAMES = 10;
constexpr float PAUSE_DELAY = 200; // in ms

int main() {
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 800, "Collision Detection Optimization", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");

    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return 1;
    }

    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    {
        Scene *currentScene;
        SceneManager *sceneManager = new SceneManager(currentScene);
        currentScene = sceneManager;

        sceneManager->registerScene<TestScene>("Test Scene");

        bool paused = true;

        double lastUpdateTime = 0;
        double currentTime = 0;
        double deltaTime = 0;

        double lowFpsFrames = 0;

        while (!glfwWindowShouldClose(window)) {
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            }

            if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
                paused = false;
            }

            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
                paused = true;
            }

            // Update
            currentTime = glfwGetTime();
            deltaTime = currentTime - lastUpdateTime;
            lastUpdateTime = currentTime;

            if (1.0f / deltaTime < MIN_FPS) {
                lowFpsFrames++;
            } else {
                lowFpsFrames = 0;
            }

            if (lowFpsFrames >= ALLOWED_LOW_FPS_FRAMES) {
                paused = true;
            }

            if (!paused && currentScene) {
                currentScene->update((float)deltaTime);
            }

            // OpenGL Rendering
            if (currentScene) {
                currentScene->render();
            }

            // ImGui Rendering
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            if (currentScene) {
                ImGui::Begin("SceneManager");

                if (currentScene != sceneManager && ImGui::Button("<-")) {
                    delete currentScene;
                    currentScene = sceneManager;
                }

                ImGui::End();

                currentScene->renderImGui();
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
