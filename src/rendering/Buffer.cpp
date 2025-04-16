#include "Buffer.h"

#include "GL/glew.h"

Buffer::Buffer() {
    glGenBuffers(1, &m_RendererID);
}

Buffer::Buffer(GLenum type, const void *data, unsigned int size, GLenum usage) {
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(type, m_RendererID);
    glBufferData(type, size, data, usage);
}

Buffer::~Buffer() {
    glDeleteBuffers(1, &m_RendererID);
}

void Buffer::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void Buffer::unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Buffer::setData(GLenum type, const void *data, unsigned int size, GLenum usage) const {
    bind();
    glBufferData(type, size, data, usage);
}
