#ifndef IMAGESCENE_H
#define IMAGESCENE_H

#include "Buffer.h"

#include "engine/Scene.h"
#include "engine/GameObject.h"
#include "engine/PhysicsSolver.h"

// #define HIGH_RESOLUTION
#define MEDIUM_RESOLUTION

class ImageScene : public Scene {
    #ifdef HIGH_RESOLUTION
    static constexpr unsigned int CIRCLE_VERTEX_COUNT = 10;
    static constexpr float CIRCLE_RADIUS = 1.0f;
    static constexpr unsigned int MAX_OBJECT_COUNT = 110000;
    static constexpr char IMAGE_PATH[] = "./res/textures/landscape_high.png";
    static constexpr int SPAWN_ROWS = 30;
    static constexpr float WORLD_SIZE = 600;
    static constexpr float GRAVITY = 80.0f;
    #endif

    #ifdef MEDIUM_RESOLUTION
    static constexpr unsigned int CIRCLE_VERTEX_COUNT = 10;
    static constexpr float CIRCLE_RADIUS = 1.0f;
    static constexpr unsigned int MAX_OBJECT_COUNT = 27000;
    static constexpr char IMAGE_PATH[] = "./res/textures/landscape_medium.jpg";
    static constexpr int SPAWN_ROWS = 20;
    static constexpr float WORLD_SIZE = 300;
    static constexpr float GRAVITY = 140.0f;
    #endif

    static constexpr float MARGIN = 10.0f;

    static constexpr float CIRCLE_DIAMETER = 2 * CIRCLE_RADIUS;

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

    bool m_StartSecondPass = false;
    int m_ImageWidth, m_ImageHeight, m_BPP;
    unsigned char *m_Image;
    glm::vec3 m_Colors[MAX_OBJECT_COUNT];

    static IndexBuffer createBoxIndexBuffer();
    static IndexBuffer createCircleIndexBuffer();

public:
    ImageScene();

    void update(float deltaTime) override;
    void render() override;
    void renderImGui() const override;

    void updateTooSlow() override;
};

#endif //IMAGESCENE_H
