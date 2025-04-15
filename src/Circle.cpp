#include "Circle.h"

#include "engine/Physics.h"

#include "glm/gtc/matrix_transform.hpp"

Circle::Circle(Renderer &renderer, VertexArray &va, IndexBuffer &ib, Shader &shader) : GameObject(renderer, va, ib, shader) { }

void Circle::update(const float deltaTime) {
    m_Velocity.y -= GRAVITY * deltaTime;

    position += m_Velocity * deltaTime;

    if (position.x < -295) {
        position.x = -295;
        m_Velocity.x = 0;
    } else if (position.x > 295) {
        position.x = 295;
        m_Velocity.x = 0;
    }

    if (position.y < -295) {
        position.y = -295;
        m_Velocity.x = 0;
    } else if (position.x > 295) {
        position.y = 295;
        m_Velocity.y = 0;
    }
}

void Circle::render() {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position.x, position.y, position.z));
    model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

    shader.bind();
    shader.setUniformMat4fv("model", model);

    renderer.draw(vao, ibo, shader, GL_TRIANGLE_FAN);
}

Vector3 Circle::getVelocity() {
    return m_Velocity;
}

void Circle::setVelocity(const Vector3 &vector) {
    m_Velocity = vector;
}
