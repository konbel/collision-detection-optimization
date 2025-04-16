#include "Renderer.h"

void Renderer::clear() const {
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::draw(const VertexArray &vao, const IndexBuffer &ibo, const Shader &shader, GLenum mode) const {
    vao.bind();
    ibo.bind();
    shader.bind();

    glDrawElements(mode, (int)ibo.getCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::drawInstanced(const VertexArray &vao, const IndexBuffer &ibo, const Shader &shader, const GLenum mode,
    const unsigned int count) const {
    vao.bind();
    ibo.bind();
    shader.bind();

    glDrawElementsInstanced(mode, ibo.getCount(), GL_UNSIGNED_INT, nullptr, count);
}
