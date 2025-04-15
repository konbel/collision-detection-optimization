#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Vector3.h"

#include "Renderer.h"

class GameObject {
protected:
    Vector3 position { 0, 0, 0 };
    Vector3 rotation { 0, 0, 0 };

    Renderer &renderer;
    VertexArray &vao;
    IndexBuffer &ibo;
    Shader &shader;

public:
    GameObject(Renderer &renderer, VertexArray &vao, IndexBuffer &ibo, Shader &shader) : renderer(renderer), vao(vao), ibo(ibo), shader(shader) {}
    virtual ~GameObject() = default;

    virtual void update(const float deltaTime) = 0;
    virtual void render() = 0;
    Vector3 getPosition() const;
    Vector3 getRotation() const;
    void setPosition(const Vector3 &vector);
    void setRotation(const Vector3 &vector);
};

#endif //GAMEOBJECT_H
