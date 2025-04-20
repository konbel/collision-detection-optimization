#ifndef SCENE_H
#define SCENE_H

#include <functional>
#include <iostream>
#include <ostream>

#include <Renderer.h>

#include <GLFW/glfw3.h>

class Scene {
protected:
    Renderer renderer;
    GLFWwindow *window;

public:
    Scene() = default;
    virtual ~Scene() = default;

    void setWindow(GLFWwindow *window);

    virtual void update(const float deltaTime) {}
    virtual void render() {}
    virtual void renderImGui() const {}

    virtual void updateTooSlow() {}
};

class SceneManager : public Scene {
    Scene *&m_CurrentScene;
    std::vector<std::pair<std::string, std::function<Scene*()>>> m_Scenes;

public:
    SceneManager(Scene *&currentScenePointer);

    void render() override;
    void renderImGui() const override;

    template<typename T>
    void registerScene(const std::string &name) {
        std::cout << "Registering scene " << name << std::endl;

        m_Scenes.push_back(std::make_pair(name, [this]() {
            T* scene = new T();
            scene->setWindow(this->window);
            return scene;
        }));
    }
};

#endif //SCENE_H
