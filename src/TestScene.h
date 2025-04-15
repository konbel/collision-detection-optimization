#ifndef TESTSCENE_H
#define TESTSCENE_H

#include "engine/Scene.h"

#include "engine/GameObject.h"

class TestScene : public Scene {
    static constexpr unsigned int CIRCLE_VERTEX_COUNT = 10;
    static constexpr float CIRCLE_RADIUS = 5.0f;
    static constexpr float SPAWN_INTERVAL = 10.0f; // in ms

    Shader m_Shader;

    VertexArray m_BoxVertexArray;
    IndexBuffer m_BoxIndexBuffer;

    VertexArray m_CircleVertexArray;
    IndexBuffer m_CircleIndexBuffer;

    std::vector<std::unique_ptr<GameObject>> m_GameObjects;
    float m_SpawnTimer = 0;

    static IndexBuffer createBoxIndexBuffer();
    static IndexBuffer createCircleIndexBuffer();

public:
    TestScene();

    void update(const float deltaTime) override;
    void render() override;
    void renderImGui() const override;
};

#endif //TESTSCENE_H
