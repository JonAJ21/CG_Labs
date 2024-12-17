#include "GL/vao.h"

namespace GL {
    VAO::VAO() {
        glGenVertexArrays(1, &mVAO);
        bind();
    }

    VAO::~VAO() {
        glDeleteBuffers(mBuffers.size(), mBuffers.data());
        glDeleteBuffers(1, &mIndicesBuffer);
        glDeleteVertexArrays(1, &mVAO);
    }

    void VAO::bind() {
        glBindVertexArray(mVAO);
    }

    void VAO::addVertexBufferObject(const std::vector<float>& data) {
        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(mBuffers.size(), 1, GL_FLOAT, GL_FALSE, 0, nullptr);
        mBuffers.push_back(vbo);
    }

    void VAO::addVertexBufferObject(const std::vector<glm::vec2>& data) {
        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(glm::vec2), data.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(mBuffers.size(), 2, GL_FLOAT, GL_FALSE, 0, nullptr);
        mBuffers.push_back(vbo);
    }

    void VAO::addVertexBufferObject(const std::vector<glm::vec3>& data) {
        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(glm::vec3), data.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(mBuffers.size(), 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        mBuffers.push_back(vbo);
    }

    void VAO::addVertexBufferObject(const std::vector<glm::vec4>& data) {
        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(glm::vec4), data.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(mBuffers.size(), 4, GL_FLOAT, GL_FALSE, 0, nullptr);
        mBuffers.push_back(vbo);
    }

    void VAO::addIndices(const std::vector<unsigned>& data) {
        assert(mIndicesBuffer == 0);
        mIndicesCount = data.size();
        glGenBuffers(1, &mIndicesBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndicesBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(unsigned), data.data(), GL_STATIC_DRAW);
    }

    void VAO::draw(unsigned type) {
        assert(mIndicesBuffer != 0);
        bind();
        for (size_t i = 0; i < mBuffers.size(); ++i) {
            glEnableVertexAttribArray(i);
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndicesBuffer);
        glDrawElements(type, mIndicesCount, GL_UNSIGNED_INT, nullptr);
        for (size_t i = 0; i < mBuffers.size(); ++i) {
            glDisableVertexAttribArray(i);
        }
    }
}