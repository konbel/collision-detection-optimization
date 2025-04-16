#include "GameObject.h"

glm::vec3 GameObject::getPosition() const {
    return position;
}

void GameObject::setPosition(const glm::vec3 &vector) {
    position = vector;
}
