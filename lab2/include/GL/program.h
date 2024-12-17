#pragma once

#include <string>
#include <glm/glm.hpp>

#include "GL/gl.h"

namespace GL {
    class Program {
    private:
        GLuint mProgram;
        GLuint mVertexShader;
        GLuint mFragmentShader;
        GLuint loadShader(const std::string& path, GLenum shaderType);
        GLuint getLocation(const std::string& name);

    public:
        Program(const std::string& path);
        ~Program();

        void link();
        void bindAttribute(GLuint index, const std::string& name);
        void use();

        void setUniform(const std::string& name, float value);
        void setUniform(const std::string& name, const glm::mat4& value);

    };
}