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

TestScene::TestScene() : m_BoxIndexBuffer(createBoxIndexBuffer()), m_BoxShader("res/shaders/basic.glsl"),
                         m_CircleIndexBuffer(createCircleIndexBuffer()), m_CircleShader("res/shaders/instanced.glsl") {

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
    circleVertexBuffer.unbind();

    m_InstanceBuffer.bind();
    unsigned int instanceBufferSize = MAX_OBJECT_COUNT * sizeof(glm::vec3);
    m_InstanceBuffer.setData(GL_ARRAY_BUFFER, nullptr, instanceBufferSize, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glVertexAttribDivisor(1, 1);

    m_CircleVertexArray.unbind();
    m_CircleIndexBuffer.unbind();
    m_InstanceBuffer.unbind();

    glm::mat4 projection = glm::ortho(-400.0f, 400.0f, -400.0f, 400.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

    m_BoxShader.bind();
    m_BoxShader.setUniformMat4fv("projection", projection);
    m_BoxShader.setUniformMat4fv("view", view);
    m_BoxShader.unbind();

    m_CircleShader.bind();
    m_CircleShader.setUniformMat4fv("projection", projection);
    m_CircleShader.setUniformMat4fv("view", view);
    m_CircleShader.unbind();
}


void TestScene::update(const float deltaTime) {
    if (m_SpawnCount < MAX_OBJECT_COUNT) {
        m_SpawnTimer += deltaTime * 1000;

        // Spawn Circles
        if (m_SpawnTimer >= SPAWN_INTERVAL) {
            m_SpawnTimer = 0;

            Circle circle(renderer, m_CircleVertexArray, m_CircleIndexBuffer, m_CircleShader);
            circle.setPosition(glm::vec3(-295, 280, 0));
            circle.setVelocity(glm::vec3(400.0f, 0.0f, 0.0f));
            m_GameObjects[m_SpawnCount] = std::make_unique<Circle>(circle);
            m_SpawnCount++;
        }
    }

    // Update GameObjects
    for (int i = 0; i < m_SpawnCount; i++) {
        m_GameObjects[i]->update(deltaTime);
    }
}

void TestScene::render() {
    renderer.clear();

    // Draw box
    renderer.draw(m_BoxVertexArray, m_BoxIndexBuffer, m_BoxShader, GL_LINE_LOOP);

    // Draw GameObjects
    if (m_SpawnCount > 0) {
        // write positions to gpu memory
        m_InstanceBuffer.bind();
        unsigned int updatedSize = m_SpawnCount * sizeof(glm::vec3);
        if (glm::vec3 *bufferPtr = (glm::vec3*)glMapBufferRange(GL_ARRAY_BUFFER, 0, updatedSize, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT)) {
            for (unsigned int i = 0; i < m_SpawnCount; i++) {
                bufferPtr[i] = m_GameObjects[i]->getPosition();
            }
            glUnmapBuffer(GL_ARRAY_BUFFER);
        }

        renderer.drawInstanced(m_CircleVertexArray, m_CircleIndexBuffer, m_CircleShader, GL_TRIANGLE_FAN, m_SpawnCount);
    }
}

void TestScene::renderImGui() const {
    ImGui::Begin("Stats");

    ImGui::Text("Elapsed time: %.1f s", glfwGetTime());
    ImGui::Text("Object count: %lu", m_SpawnCount);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::End();
}
