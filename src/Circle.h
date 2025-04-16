#ifndef CIRCLE_H
#define CIRCLE_H

#include "engine/GameObject.h"

class Circle : public GameObject {
    glm::vec3 m_Velocity;

public:
    Circle(Renderer &renderer, VertexArray &va, IndexBuffer &ib, Shader &shader);

    void update(const float deltaTime) override;
    void render() override;

    glm::vec3 getVelocity() const;
    void setVelocity(const glm::vec3 &vector);
};

#endif //CIRCLE_H
