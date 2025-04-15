#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

class IndexBuffer {
    unsigned int m_RendererID;
    unsigned int m_Count;

public:
    IndexBuffer(const unsigned int *indices, long count);
    ~IndexBuffer();

    void bind() const;
    void unbind() const;

    inline unsigned int getCount() const { return m_Count;}
};

#endif //INDEXBUFFER_H
