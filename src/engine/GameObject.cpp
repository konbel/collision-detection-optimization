#include "GameObject.h"

Vector3 GameObject::getPosition() const {
    return position;
}

Vector3 GameObject::getRotation() const {
    return rotation;
}

void GameObject::setPosition(const Vector3 &vector) {
    position = vector;
}

void GameObject::setRotation(const Vector3 &vector) {
    rotation = Vector3(vector.x * M_PI / 180, vector.y * M_PI / 180, vector.z * M_PI / 180);
}
