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
    } else if (position.y > 295) {
        position.y = 295;
        m_Velocity.x = 0;
    }
}

void Circle::render() {
    renderer.draw(vao, ibo, shader, GL_TRIANGLE_FAN);
}

glm::vec3 Circle::getVelocity() const {
    return m_Velocity;
}

void Circle::setVelocity(const glm::vec3 &vector) {
    m_Velocity = vector;
}
