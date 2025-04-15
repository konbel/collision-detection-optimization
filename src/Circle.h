#ifndef CIRCLE_H
#define CIRCLE_H

#include "engine/GameObject.h"

class Circle : public GameObject {
    Vector3 m_Velocity;

public:
    Circle(Renderer &renderer, VertexArray &va, IndexBuffer &ib, Shader &shader);

    void update(const float deltaTime) override;
    void render() override;

    Vector3 getVelocity();
    void setVelocity(const Vector3 &vector);
};

#endif //CIRCLE_H
