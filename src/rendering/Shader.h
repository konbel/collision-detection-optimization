#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <unordered_map>

#include "glm/gtc/matrix_transform.hpp"

struct ShaderProgramSource {
    std::string vertexSource;
    std::string fragmentSource;
};

class Shader {
    unsigned int m_RendererID;
    std::unordered_map<std::string, int> m_UniformLocationCache;

public:
    Shader(const char *filepath);
    ~Shader();

    void bind() const;
    void unbind() const;

    void setUniformMat4fv(const std::string &name, const glm::mat4 &matrix);

private:
    static ShaderProgramSource parseShader(const char* filepath);
    static unsigned int compileShader(unsigned int shaderType, const std::string &shaderSource);
    static unsigned int createProgram(const char* filepath);

    int getUniformLocation(const std::string &name);
};

#endif //SHADER_H
