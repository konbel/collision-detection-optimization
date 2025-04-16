#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

class Renderer {
public:
    void clear() const;
    void draw(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader, GLenum mode) const;
    void drawInstanced(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader, const GLenum mode, const unsigned int count) const;
};

#endif //RENDERER_H
