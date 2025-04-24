#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "TestScene.h"
#include "ImageScene.h"

#define USE_MIN_FPS 0                           // Note: Using min FPS may affect scene behavior under fluctuating framerate.
static constexpr double MIN_FPS = 1 / 60.0f;    //       See main loop comment for more details.
static constexpr double FPS_CAP = 1 / 60.0f;

int main() {
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(1920, 1080, "Collision Detection Optimization", NULL, NULL);
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
        sceneManager->setWindow(window);
        currentScene = sceneManager;

        sceneManager->registerScene<TestScene>("Test Scene");
        sceneManager->registerScene<ImageScene>("Image Scene");

        double lastUpdateTime = 0;
        double lastFrameTime = 0;
        double currentFrameTime = 0;
        double deltaTime = 0;

        while (!glfwWindowShouldClose(window)) {
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            }

            // Update
            currentFrameTime = glfwGetTime();

            /*
             * Checks if the framerate has dropped below the specified MIN_FPS.
             * If it has, notifies the current scene. How the scene responds to this is up to its implementation.
             *
             * Note: This mechanism does not guarantee deterministic behavior.
             *       The overall sequence of steps remains the same, but the exact number of updates or spawns
             *       may vary depending on when the framerate dips below the minimum.
             */
            #if USE_MIN_FPS
                if (currentFrameTime - lastFrameTime > MIN_FPS) {
                    if (currentScene) {
                        currentScene->updateTooSlow();
                    }
                }
            #endif

            lastFrameTime = currentFrameTime;

            // Scene update loop running at fixed framerate to guarantee deterministic physics.
            deltaTime = currentFrameTime - lastUpdateTime;
            if (deltaTime > FPS_CAP) {
                lastUpdateTime = currentFrameTime;
                if (currentScene) {
                    currentScene->update(FPS_CAP);
                }
            }

            // Rendering independent of the specified FPS_CAP
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

        if (currentScene != sceneManager) {
            delete sceneManager;
        }

        delete currentScene;
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
