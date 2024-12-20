#include "Engine/Geometry/2D/polygon.h"

#include <glm/ext/scalar_constants.hpp>
#include <iostream>
namespace Engine {
    Polygon::Polygon(const std::vector<glm::vec3>& vertices,
                     const std::vector<glm::vec3>& colors,
                     const std::vector<unsigned int>& indices,
                     GL::Program* program)
        : mVertices(vertices),
          mColors(colors),
          mIndices(indices),
          mProgram(program),
          mTranslationMatrix(glm::mat4(1.0f)),
          mRotationMatrix(glm::mat4(1.0f)),
          mScaleMatrix(glm::mat4(1.0f))
    {
        mVAO.addVertexBufferObject(mVertices);
        mVAO.addVertexBufferObject(mColors);
        mVAO.addIndices(mIndices);
    }

    Polygon::~Polygon() {
        delete mProgram;
    }


    glm::mat4 Polygon::getModelMatrix() const {
        return mTranslationMatrix * mRotationMatrix * mScaleMatrix;
    }

    void Polygon::draw() {
        mVAO.draw(GL_POLYGON);
    }

    void Polygon::move(const glm::vec3& translation) {
        glm::mat4 tempTranslationMatrix = mTranslationMatrix;

        tempTranslationMatrix[3][0] += translation[0];
        tempTranslationMatrix[3][1] += translation[1];
        tempTranslationMatrix[3][2] += translation[2];
        for (auto vec3 : mVertices) {
            glm::vec4 pos = { vec3.x, vec3.y, vec3.z, 1.0f };
            pos = tempTranslationMatrix * mRotationMatrix * mScaleMatrix * pos;
            if (pos.x > 1 || pos.x < -1 || 
                pos.y > 1 || pos.y < -1 ||
                pos.z > 1 || pos.z < -1) {
                return;
            }
        }
        mTranslationMatrix[3][0] += translation[0];
        mTranslationMatrix[3][1] += translation[1];
        mTranslationMatrix[3][2] += translation[2];
    }

    void Polygon::rotate(const glm::vec3& angles) {
        glm::mat4 rotX = {
            {1.0f, 0.0f, 0.0f, 0.f},
            {0.0f, glm::cos(glm::pi<float>() * angles[0] / 180), -glm::sin(glm::pi<float>() * angles[0] / 180), 0.0f},
            {0.0f, glm::sin(glm::pi<float>() * angles[0] / 180), glm::cos(glm::pi<float>() * angles[0] / 180), 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}
        };
        glm::mat4 rotY = {
            {glm::cos(glm::pi<float>() * angles[1] / 180), 0.0f, glm::sin(glm::pi<float>() * angles[1] / 180), 0.0f},
            {0.0f, 1.0f, 0.0f, 0.0f},
            {-glm::sin(glm::pi<float>() * angles[1] / 180), 0.0f, glm::cos(glm::pi<float>() * angles[1] / 180), 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}
        };
        glm::mat4 rotZ = {
            {glm::cos(glm::pi<float>() * angles[2] / 180), -glm::sin(glm::pi<float>() * angles[2] / 180), 0.0f, 0.0f},
            {glm::sin(glm::pi<float>() * angles[2] / 180), glm::cos(glm::pi<float>() * angles[2] / 180), 0.0f, 0.0f},
            {0.0f, 0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}
        };

        glm::mat4 tempRotationMatrix = mRotationMatrix;

        tempRotationMatrix *= rotZ * rotY * rotX;

        for (auto vec3 : mVertices) {
            glm::vec4 pos = { vec3.x, vec3.y, vec3.z, 1.0f };
            pos = tempRotationMatrix * mTranslationMatrix * mScaleMatrix * pos;
            if (pos.x > 1 || pos.x < -1 || 
                pos.y > 1 || pos.y < -1 ||
                pos.z > 1 || pos.z < -1) {
                return;
            }
        }
        mRotationMatrix = tempRotationMatrix;
    }

    void Polygon::scale(const float& scale) {
        glm::mat4 tempScaleMatrix = mScaleMatrix;
        tempScaleMatrix[0][0] *= scale;
        tempScaleMatrix[1][1] *= scale;
        tempScaleMatrix[2][2] *= scale;

        for (auto vec3 : mVertices) {
            glm::vec4 pos = { vec3.x, vec3.y, vec3.z, 1.0f };
            pos = mTranslationMatrix * mRotationMatrix * tempScaleMatrix * pos;
            if (pos.x > 1 || pos.x < -1 ||
                pos.y > 1 || pos.y < -1 ||
                pos.z > 1 || pos.z < -1) {
                return;
            }
        }

        mScaleMatrix[0][0] *= scale;
        mScaleMatrix[1][1] *= scale;
        mScaleMatrix[2][2] *= scale;
    }

    void Polygon::scale(const glm::vec3& scale) {
        glm::mat4 tempScaleMatrix = mScaleMatrix;
        tempScaleMatrix[0][0] *= scale[0];
        tempScaleMatrix[1][1] *= scale[1];
        tempScaleMatrix[2][2] *= scale[2];

        for (auto vec3 : mVertices) {
            glm::vec4 pos = { vec3.x, vec3.y, vec3.z, 1.0f };
            pos = mTranslationMatrix * mRotationMatrix * tempScaleMatrix * pos;
            if (pos.x > 1 || pos.x < -1 ||
                pos.y > 1 || pos.y < -1 ||
                pos.z > 1 || pos.z < -1) {
                return;
            }   
        }

        mScaleMatrix[0][0] *= scale[0];
        mScaleMatrix[1][1] *= scale[1];
        mScaleMatrix[2][2] *= scale[2];
    }
}