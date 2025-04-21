#include "GameObject.h"

GameObject::GameObject(const unsigned int id, const glm::vec2 &position) : m_Id(id), position(position), lastPosition(position) { }

unsigned int GameObject::getId() const {
    return m_Id;
}

void GameObject::update(const float deltaTime) {
    const glm::vec2 velocity = position - lastPosition;

    constexpr float VELOCITY_DAMPING = 40.0f;

    const glm::vec2 newPosition = position + velocity + (acceleration - velocity * VELOCITY_DAMPING) * (deltaTime * deltaTime);
    lastPosition = position;
    position = newPosition;
    acceleration = glm::vec2{ 0 };
}
