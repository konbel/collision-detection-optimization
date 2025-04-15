#include "Scene.h"

#include <imgui.h>

SceneManager::SceneManager(Scene *&currentScenePointer) : m_CurrentScene(currentScenePointer) {
}

void SceneManager::renderImGui() const {
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui::Begin("SceneManager");

    for (auto &scene : m_Scenes) {
        if (ImGui::Button(scene.first.c_str())) {
            m_CurrentScene = scene.second();
        }
    }

    ImGui::End();
}
