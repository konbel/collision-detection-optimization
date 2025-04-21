#ifndef TESTSCENE_H
#define TESTSCENE_H

#include "Buffer.h"

#include "engine/Scene.h"
#include "engine/GameObject.h"
#include "engine/PhysicsSolver.h"

class TestScene : public Scene {
    static constexpr unsigned int CIRCLE_VERTEX_COUNT = 10;
    static constexpr float MARGIN = 10.0f;
    static constexpr float COLOR_CYCLE_SPEED = 0.0001;

    static constexpr float WORLD_SIZE = 300;
    static constexpr float CIRCLE_RADIUS = 1.0f;
    static constexpr float CIRCLE_DIAMETER = 2 * CIRCLE_RADIUS;
    static constexpr float GRAVITY = 140.0f;
    static constexpr unsigned int MAX_OBJECT_COUNT = 10000;

    VertexArray m_BoxVertexArray;
    IndexBuffer m_BoxIndexBuffer;
    Shader m_BoxShader;

    VertexArray m_CircleVertexArray;
    IndexBuffer m_CircleIndexBuffer;
    Buffer m_InstanceBuffer;
    Shader m_CircleShader;

    PhysicsSolver m_PhysicsSolver;

    bool m_Paused = false;

    bool m_Emit = false;
    int m_SpawnRows = 8;

    static IndexBuffer createBoxIndexBuffer();
    static IndexBuffer createCircleIndexBuffer();

    static glm::vec3 getRainbowColor(float t);

public:
    TestScene();

    void update(float deltaTime) override;
    void render() override;
    void renderImGui() const override;

    void updateTooSlow() override;
};

#endif //TESTSCENE_H
