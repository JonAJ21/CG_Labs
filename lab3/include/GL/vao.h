#pragma once

#include <vector>

#include "GL/gl.h"
#include <glm/glm.hpp>

namespace GL {
    class VAO {
    private:
        GLuint mVAO;
        std::vector<GLuint> mBuffers;
        unsigned mIndicesBuffer = 0;
        size_t mIndicesCount = 0;

    public:
        VAO();
        VAO(const VAO&) = delete;
        ~VAO();

        void bind();
        void addVertexBufferObject(const std::vector<float>& data);
        void addVertexBufferObject(const std::vector<glm::vec2>& data);
        void addVertexBufferObject(const std::vector<glm::vec3>& data);
        void addVertexBufferObject(const std::vector<glm::vec4>& data);
        void addIndices(const std::vector<unsigned>& data);

        void draw(unsigned type = GL_TRIANGLES);
    };
}