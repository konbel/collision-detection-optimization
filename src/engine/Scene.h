#ifndef SCENE_H
#define SCENE_H

#include <functional>
#include <iostream>
#include <ostream>

#include <Renderer.h>

class Scene {
protected:
    Renderer renderer;

public:
    Scene() = default;
    virtual ~Scene() = default;

    virtual void update(const float deltaTime) {}
    virtual void render() {}
    virtual void renderImGui() const {}
};

class SceneManager : public Scene {
    Scene *&m_CurrentScene;
    std::vector<std::pair<std::string, std::function<Scene*()>>> m_Scenes;

public:
    SceneManager(Scene *&currentScenePointer);

    void renderImGui() const override;

    template<typename T>
    void registerScene(const std::string &name) {
        std::cout << "Registering scene " << name << std::endl;
        m_Scenes.push_back(std::make_pair(name, []() { return new T(); }));
    }
};

#endif //SCENE_H
