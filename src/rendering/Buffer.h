#ifndef BUFFER_H
#define BUFFER_H

#include <GL/glew.h>

class Buffer {
    unsigned int m_RendererID;
    
public:
    Buffer();
    Buffer(GLenum type, const void *data, unsigned int size, GLenum usage);
    ~Buffer();

    void bind() const;
    void unbind() const;
    void setData(GLenum type, const void *data, unsigned int size, GLenum usage) const;
};

#endif //BUFFER_H
