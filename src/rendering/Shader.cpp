#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "Renderer.h"

Shader::Shader(const char *filepath) {
    m_RendererID = createProgram(filepath);
}

Shader::~Shader() {
    glDeleteProgram(m_RendererID);
}

void Shader::bind() const {
    glUseProgram(m_RendererID);
}

void Shader::unbind() const {
    glUseProgram(0);
}

void Shader::setUniformMat4fv(const std::string &name, const glm::mat4 &matrix) {
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

ShaderProgramSource Shader::parseShader(const char* filepath) {
    std::ifstream stream(filepath);

    enum ShaderType {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1,
    };

    ShaderType type = NONE;

    std::stringstream ss[2];
    std::string line;
    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = VERTEX;
            } else if (line.find("fragment") != std::string::npos) {
                type = FRAGMENT;
            } else {
                type = NONE;
            }
        } else {
            ss[(int)type] << line << "\n";
        }
    }

    return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::compileShader(GLenum shaderType, const std::string &shaderSource) {
    unsigned int shader = glCreateShader(shaderType);
    const char *src = shaderSource.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (compiled == GL_FALSE) {
        char message[1024];
        glGetShaderInfoLog(shader, 1024, nullptr, message);
        std::cout << message << std::endl;
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

unsigned int Shader::createProgram(const char* filepath) {
    ShaderProgramSource source = parseShader(filepath);
    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, source.vertexSource);
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, source.fragmentSource);

    unsigned int program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    int programLinked;
    glGetProgramiv(program, GL_LINK_STATUS, &programLinked);
    if (programLinked == GL_FALSE) {
        char message[1024];
        glGetProgramInfoLog(program, 1024, nullptr, message);
        glDeleteProgram(program);
        return 0;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

int Shader::getUniformLocation(const std::string &name) {
    if (m_UniformLocationCache.contains(name)) {
        return m_UniformLocationCache[name];
    }

    int location = glGetUniformLocation(m_RendererID, name.c_str());
    if (location == -1) {
        std::cout << "Uniform location for " << name << " not found" << std::endl;
    }
    m_UniformLocationCache[name] = location;

    return location;
}
