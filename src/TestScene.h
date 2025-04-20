#ifndef TESTSCENE_H
#define TESTSCENE_H

#include "Buffer.h"

#include "engine/Scene.h"
#include "engine/GameObject.h"
#include "engine/PhysicsSolver.h"

class TestScene : public Scene {
    static constexpr unsigned int CIRCLE_VERTEX_COUNT = 10;

    static constexpr float WORLD_SIZE = 300;
    static constexpr float CIRCLE_RADIUS = 2.0f;
    static constexpr float CIRCLE_DIAMETER = 2 * CIRCLE_RADIUS;
    static constexpr float GRAVITY = -200.0f;

    static constexpr unsigned int MAX_OBJECT_COUNT = 1000;

    static constexpr float COLOR_CYCLE_SPEED = 0.001;

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
    int m_SpawnRows = 2;
    glm::vec3 m_Color = glm::vec3(0.0f, 1.0f, 1.0f);

    static IndexBuffer createBoxIndexBuffer();
    static IndexBuffer createCircleIndexBuffer();

    glm::vec3 getRainbowColor();

public:
    TestScene();

    void update(float deltaTime) override;
    void render() override;
    void renderImGui() const override;

    void updateTooSlow() override;
};

#endif //TESTSCENE_H
