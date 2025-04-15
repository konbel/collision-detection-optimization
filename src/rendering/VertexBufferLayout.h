#ifndef VERTEXBUFFERLAYOUT_H
#define VERTEXBUFFERLAYOUT_H

#include <vector>
#include <GL/glew.h>

struct VertexBufferElement {
    unsigned int count;
    GLenum type;
    unsigned char normalized;

    unsigned int getSizeOfType() const {
        switch (type) {
            case GL_FLOAT:          return 4;
            case GL_UNSIGNED_INT:   return 4;
            case GL_UNSIGNED_BYTE:  return 1;
            default:                return 0;
        }
    }
};

class VertexBufferLayout {
    std::vector<VertexBufferElement> m_Elements;
    unsigned int m_Stride;

public:
    VertexBufferLayout() : m_Stride(0) {}

    template<typename T>
    void push(unsigned int count) {
        static_assert(false);
    }

    inline const std::vector<VertexBufferElement> getElements() const {
        return m_Elements;
    }

    inline unsigned int getStride() const {
        return m_Stride;
    }
};

template<>
inline void VertexBufferLayout::push<float>(unsigned int count) {
    m_Elements.push_back({ count, GL_FLOAT, GL_FALSE });
    m_Stride += count * sizeof(float);
}

#endif //VERTEXBUFFERLAYOUT_H
