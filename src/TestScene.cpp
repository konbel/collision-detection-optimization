#include "TestScene.h"

#include "imgui.h"

#include "Circle.h"

IndexBuffer TestScene::createBoxIndexBuffer() {
    unsigned int boxIndices[] = {
        0, 1, 2, 3,
    };

    return IndexBuffer(boxIndices, 4);
}

IndexBuffer TestScene::createCircleIndexBuffer() {
    unsigned int indices[CIRCLE_VERTEX_COUNT * 3];

    for (int i = 0; i < CIRCLE_VERTEX_COUNT - 1; i++) {
        indices[i * 3] = 0;
        indices[i * 3 + 1] = i + 1;
        indices[i * 3 + 2] = i + 2;
    }

    return IndexBuffer(indices, CIRCLE_VERTEX_COUNT * 3);
}

TestScene::TestScene() : m_Shader("res/shaders/basic.glsl"), m_BoxVertexArray(), m_BoxIndexBuffer(createBoxIndexBuffer()),
    m_CircleVertexArray(), m_CircleIndexBuffer(createCircleIndexBuffer()) {

    VertexBufferLayout layout;
    layout.push<float>(2);

    m_BoxVertexArray.bind();

    float boxVertices[] = {
        -300.0f, -300.0f,
         300.0f, -300.0f,
         300.0f,  300.0f,
        -300.0f,  300.0f,
    };

    VertexBuffer boxVertexBuffer(boxVertices, 4 * 2 * sizeof(float));
    m_BoxVertexArray.addBuffer(boxVertexBuffer, layout);

    m_BoxVertexArray.unbind();
    boxVertexBuffer.unbind();
    m_BoxIndexBuffer.unbind();

    m_CircleVertexArray.bind();

    unsigned int vertexCount = 10;
    float circleVertices[vertexCount * 2];
    circleVertices[0] = 0.0f;
    circleVertices[1] = 0.0f;

    for (int i = 1; i < vertexCount; i++) {
        float angle = 2.0f * 3.14159f * ((float)i - 1) / ((float)vertexCount - 2.0f);
        circleVertices[i * 2] = CIRCLE_RADIUS * cosf(angle);
        circleVertices[i * 2 + 1] = CIRCLE_RADIUS * sinf(angle);
    }

    VertexBuffer circleVertexBuffer(circleVertices, CIRCLE_VERTEX_COUNT * 2 * sizeof(float));
    m_CircleVertexArray.addBuffer(circleVertexBuffer, layout);

    m_CircleVertexArray.unbind();
    circleVertexBuffer.unbind();
    m_CircleIndexBuffer.unbind();

    glm::mat4 projection = glm::ortho(-400.0f, 400.0f, -400.0f, 400.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

    m_Shader.bind();
    m_Shader.setUniformMat4fv("projection", projection);
    m_Shader.setUniformMat4fv("view", view);
    m_Shader.unbind();
}


void TestScene::update(const float deltaTime) {
    m_SpawnTimer += deltaTime * 1000;

    // Spawn Circles
    if (m_SpawnTimer >= SPAWN_INTERVAL) {
        m_SpawnTimer = 0;

        Circle circle(renderer, m_CircleVertexArray, m_CircleIndexBuffer, m_Shader);
        circle.setPosition(Vector3(-295, 280, 0));
        circle.setVelocity(Vector3(400.0f, 0.0f, 0.0f));
        m_GameObjects.push_back(std::make_unique<Circle>(circle));
    }

    // Update GameObjects
    for (const auto &gameObject : m_GameObjects) {
        gameObject->update((float)deltaTime);
    }
}

void TestScene::render() {
    renderer.clear();

    // Draw box
    m_Shader.bind();
    m_Shader.setUniformMat4fv("model", glm::mat4(1.0f));
    renderer.draw(m_BoxVertexArray, m_BoxIndexBuffer, m_Shader, GL_LINE_LOOP);

    // Draw GameObjects
    for (const auto &gameObject : m_GameObjects) {
        gameObject->render();
    }
}

void TestScene::renderImGui() const {
    ImGui::Begin("Stats");

    ImGui::Text("Elapsed time: %.1f s", glfwGetTime());
    ImGui::Text("Object count: %lu", m_GameObjects.size());
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::End();
}
