#include "ImageScene.h"

#include "imgui.h"

#include "stb-image/stb_image.h"

struct InstanceData {
    glm::vec2 position;
    glm::vec3 color;
};

IndexBuffer ImageScene::createBoxIndexBuffer() {
    unsigned int boxIndices[] = {
        0, 1, 2, 3,
    };

    return IndexBuffer(boxIndices, 4);
}

IndexBuffer ImageScene::createCircleIndexBuffer() {
    unsigned int indices[CIRCLE_VERTEX_COUNT * 3];

    for (int i = 0; i < CIRCLE_VERTEX_COUNT - 1; i++) {
        indices[i * 3] = 0;
        indices[i * 3 + 1] = i + 1;
        indices[i * 3 + 2] = i + 2;
    }

    return IndexBuffer(indices, CIRCLE_VERTEX_COUNT * 3);
}

ImageScene::ImageScene() : m_BoxIndexBuffer(createBoxIndexBuffer()), m_BoxShader("res/shaders/basic.glsl"),
                         m_CircleIndexBuffer(createCircleIndexBuffer()), m_CircleShader("res/shaders/instanced.glsl"),
                         m_PhysicsSolver(WORLD_SIZE, CIRCLE_RADIUS, CIRCLE_DIAMETER, GRAVITY, MAX_OBJECT_COUNT) {
    VertexBufferLayout layout;
    layout.push<float>(2);

    m_BoxVertexArray.bind();

    float boxVertices[] = {
        0, 0,
        WORLD_SIZE, 0,
        WORLD_SIZE, WORLD_SIZE,
        0, WORLD_SIZE,
    };

    VertexBuffer boxVertexBuffer(boxVertices, 4 * 2 * sizeof(float));
    m_BoxVertexArray.addBuffer(boxVertexBuffer, layout);

    m_BoxVertexArray.unbind();
    boxVertexBuffer.unbind();
    m_BoxIndexBuffer.unbind();

    m_CircleVertexArray.bind();

    float circleVertices[CIRCLE_VERTEX_COUNT * 2];
    circleVertices[0] = 0.0f;
    circleVertices[1] = 0.0f;

    for (int i = 1; i < CIRCLE_VERTEX_COUNT; i++) {
        const float angle = 2.0f * 3.14159f * ((float)i - 1) / ((float)CIRCLE_VERTEX_COUNT - 2.0f);
        circleVertices[i * 2] = CIRCLE_RADIUS * cosf(angle);
        circleVertices[i * 2 + 1] = CIRCLE_RADIUS * sinf(angle);
    }

    VertexBuffer circleVertexBuffer(circleVertices, CIRCLE_VERTEX_COUNT * 2 * sizeof(float));
    m_CircleVertexArray.addBuffer(circleVertexBuffer, layout);
    circleVertexBuffer.unbind();

    m_InstanceBuffer.bind();
    constexpr unsigned int INSTANCE_BUFFER_SIZE = MAX_OBJECT_COUNT * sizeof(InstanceData);
    m_InstanceBuffer.setData(GL_ARRAY_BUFFER, nullptr, INSTANCE_BUFFER_SIZE, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)0);
    glVertexAttribDivisor(1, 1);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)sizeof(glm::vec2));
    glVertexAttribDivisor(2, 1);

    m_CircleVertexArray.unbind();
    m_CircleIndexBuffer.unbind();
    m_InstanceBuffer.unbind();

    static constexpr  float ASPECT_RATIO = 1920.0f / 1080.0f;
    static constexpr float VIEWPORT_HEIGHT = MARGIN * 2 + WORLD_SIZE;
    static constexpr float VIEWPORT_WIDTH = VIEWPORT_HEIGHT * ASPECT_RATIO;

    const glm::mat4 projection = glm::ortho(0.0f, VIEWPORT_WIDTH, VIEWPORT_HEIGHT, 0.0f);
    const glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(VIEWPORT_WIDTH / 2 - WORLD_SIZE / 2, VIEWPORT_HEIGHT / 2 - WORLD_SIZE / 2, 0.0f));

    m_BoxShader.bind();
    m_BoxShader.setUniformMat4fv("projection", projection);
    m_BoxShader.setUniformMat4fv("view", view);
    m_BoxShader.unbind();

    m_CircleShader.bind();
    m_CircleShader.setUniformMat4fv("projection", projection);
    m_CircleShader.setUniformMat4fv("view", view);
    m_CircleShader.unbind();

    m_Image = stbi_load(IMAGE_PATH, &m_ImageWidth, &m_ImageHeight, &m_BPP, 3);

    for (glm::vec3 &color : m_Colors) {
        color = glm::vec3(1.0f);
    }
}

void ImageScene::update(const float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        m_Paused = false;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        m_Paused = true;
    }

    if (m_Paused) {
        return;
    }

    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
        m_Emit = true;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        m_Emit = false;
    }

    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
        m_StartSecondPass = true;
    } else {
        m_StartSecondPass = false;
    }

    // Spawn objects
    if (m_Emit) {
        for (int i = 0; i < SPAWN_ROWS; i++) {
            if (const int id = m_PhysicsSolver.addObject(glm::vec2(CIRCLE_RADIUS, 10 + (CIRCLE_DIAMETER + 0.1f) * i)); id != -1) {
                GameObject &object = m_PhysicsSolver.objects[id];
                object.lastPosition.x -= CIRCLE_RADIUS * 0.25f + 0.1f;
                object.color = m_Colors[object.getId()];
            }
        }
    }

    if (m_PhysicsSolver.count == MAX_OBJECT_COUNT && m_StartSecondPass) {
        // assign colors
        if ((int)(WORLD_SIZE / CIRCLE_DIAMETER) != m_ImageWidth || (int)(WORLD_SIZE / CIRCLE_DIAMETER) != m_ImageHeight) {
            std::cout << "Image dimensions don't match world and circle size. May cause unpredictable behavior!" <<
                         std::endl << "Image pixel = world size / circle diameter" << std::endl;
        }

        for (int i = 0; i < m_PhysicsSolver.count; i++) {
            const auto &object = m_PhysicsSolver.objects[i];
            auto &color = m_Colors[i];

            const int x = (int)(object.position.x - CIRCLE_RADIUS) / (int)CIRCLE_DIAMETER;
            const int y = (int)(object.position.y - CIRCLE_RADIUS) / (int)CIRCLE_DIAMETER;
            const int idx = (y * m_ImageWidth + x) * 3;
            color = glm::vec3(m_Image[idx] / 255.0f, m_Image[idx + 1] / 255.0f, m_Image[idx + 2] / 255.0f);
        }

        // rerun
        m_PhysicsSolver.objects.clear();
        m_PhysicsSolver.count = 0;
    }

    m_PhysicsSolver.update(deltaTime);
}

void ImageScene::render() {
    renderer.clear();

    // Draw box
    renderer.draw(m_BoxVertexArray, m_BoxIndexBuffer, m_BoxShader, GL_LINE_LOOP);

    // Draw GameObjects
    if (m_PhysicsSolver.count > 0) {
        // write positions to gpu memory
        m_InstanceBuffer.bind();
        const unsigned int updatedSize = m_PhysicsSolver.count * sizeof(InstanceData);
        if (InstanceData *bufferPtr = (InstanceData*)glMapBufferRange(GL_ARRAY_BUFFER, 0, updatedSize, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT)) {
            for (unsigned int i = 0; i < m_PhysicsSolver.count; i++) {
                bufferPtr[i].position = m_PhysicsSolver.objects[i].position;
                bufferPtr[i].color = m_PhysicsSolver.objects[i].color;
            }
            glUnmapBuffer(GL_ARRAY_BUFFER);
        }

        renderer.drawInstanced(m_CircleVertexArray, m_CircleIndexBuffer, m_CircleShader, GL_TRIANGLE_FAN, m_PhysicsSolver.count);
    }
}

void ImageScene::renderImGui() const {
    ImGui::Begin("Stats");

    ImGui::Text("Elapsed time: %.1f s", glfwGetTime());
    ImGui::Text("Object count: %d", m_PhysicsSolver.count);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::End();

    ImGui::Begin("Controls");

    ImGui::Text("Pause                                      S");
    ImGui::Text("Resume                                     R");
    ImGui::Text("Stop Emit                              Space");
    ImGui::Text("Resume Emit                            Enter");
    ImGui::Text("Start Image (only when first done)         H");

    ImGui::End();
}

void ImageScene::updateTooSlow() {
    m_Emit = false;
}
