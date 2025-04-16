#ifndef TESTSCENE_H
#define TESTSCENE_H

#include "Buffer.h"

#include "engine/Scene.h"
#include "engine/GameObject.h"

class TestScene : public Scene {
    static constexpr unsigned int CIRCLE_VERTEX_COUNT = 10;
    static constexpr float CIRCLE_RADIUS = 5.0f;
    static constexpr float SPAWN_INTERVAL = 10.0f; // in ms
    static constexpr unsigned int MAX_OBJECT_COUNT = 100000;

    VertexArray m_BoxVertexArray;
    IndexBuffer m_BoxIndexBuffer;
    Shader m_BoxShader;

    VertexArray m_CircleVertexArray;
    IndexBuffer m_CircleIndexBuffer;
    Buffer m_InstanceBuffer;
    Shader m_CircleShader;

    std::unique_ptr<GameObject> m_GameObjects[MAX_OBJECT_COUNT];
    float m_SpawnTimer = 0;
    unsigned int m_SpawnCount = 0;

    static IndexBuffer createBoxIndexBuffer();
    static IndexBuffer createCircleIndexBuffer();

public:
    TestScene();

    void update(const float deltaTime) override;
    void render() override;
    void renderImGui() const override;
};

#endif //TESTSCENE_H
