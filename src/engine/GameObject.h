#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Renderer.h"

class GameObject {
protected:
    glm::vec3 position { 0, 0, 0 };

    Renderer &renderer;
    VertexArray &vao;
    IndexBuffer &ibo;
    Shader &shader;

public:
    GameObject(Renderer &renderer, VertexArray &vao, IndexBuffer &ibo, Shader &shader) : renderer(renderer), vao(vao), ibo(ibo), shader(shader) {}
    virtual ~GameObject() = default;

    virtual void update(const float deltaTime) = 0;
    virtual void render() = 0;

    glm::vec3 getPosition() const;
    void setPosition(const glm::vec3 &vector);
};

#endif //GAMEOBJECT_H
