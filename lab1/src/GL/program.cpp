#include <fstream>

#include "GL/program.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace GL {
    Program::Program(const std::string& path) {
        mProgram = glCreateProgram();
        mVertexShader = loadShader(path + ".vert", GL_VERTEX_SHADER);
        mFragmentShader = loadShader(path + ".frag", GL_FRAGMENT_SHADER);
    }

    Program::~Program() {
        glDetachShader(mProgram, mVertexShader);
        glDetachShader(mProgram, mFragmentShader);
        glDeleteShader(mVertexShader);
        glDeleteShader(mFragmentShader);
        glDeleteProgram(mProgram);
    }

    GLuint Program::loadShader(const std::string& path, GLenum shaderType) {
        GLuint shader = glCreateShader(shaderType);

        std::ifstream file(path);
        std::string code((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

        const char* codePtr = code.c_str();
        glShaderSource(shader, 1, &codePtr, nullptr);
        glCompileShader(shader);
        GLint status;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        if (status == GL_FALSE) {
            GLint infoLogLength;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
            std::string infoLog(infoLogLength, ' ');
            glGetShaderInfoLog(shader, infoLogLength, nullptr, &infoLog[0]);
            throw std::runtime_error("Shader compilation failed: " + infoLog);
        }

        return shader;
    }

    void Program::link() {
        glAttachShader(mProgram, mVertexShader);
        glAttachShader(mProgram, mFragmentShader);
        glLinkProgram(mProgram);

        GLint status;
        glGetProgramiv(mProgram, GL_LINK_STATUS, &status);
        if (status == GL_FALSE) {
            throw std::runtime_error("Program linking failed");
        }
    }

    void Program::bindAttribute(GLuint index, const std::string& name) {
        glBindAttribLocation(mProgram, index, name.c_str());
    }

    void Program::use() {
        glUseProgram(mProgram);
    }

    GLuint Program::getLocation(const std::string& name) {
        return glGetUniformLocation(mProgram, name.c_str());
    }

    void Program::setUniform(const std::string& name, float value) {
        glUniform1f(getLocation(name), value);
    }

    void Program::setUniform(const std::string& name, const glm::mat4& value) {
        glUniformMatrix4fv(getLocation(name), 1, GL_FALSE, glm::value_ptr(value));
    }
}