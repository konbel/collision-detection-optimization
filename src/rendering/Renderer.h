#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

class Renderer {
public:
    void clear() const;
    void draw(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader, GLenum mode) const;
};

#endif //RENDERER_H
