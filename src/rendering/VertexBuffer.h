#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

class VertexBuffer {
    unsigned int m_RendererID;

public:
    VertexBuffer(const void *vertices, unsigned int size);
    ~VertexBuffer();

    void bind() const;
    void unbind() const;
};

#endif //VERTEXBUFFER_H
