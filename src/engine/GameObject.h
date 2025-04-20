#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Renderer.h"

class GameObject {
    unsigned int m_Id;

public:
    glm::vec2 position { 0 };
    glm::vec2 lastPosition { 0 };
    glm::vec2 acceleration { 0 };

    glm::vec3 color { 1 };

    GameObject(unsigned int id, const glm::vec2 &position);
    ~GameObject() = default;

    void update(float deltaTime);
};

#endif //GAMEOBJECT_H
