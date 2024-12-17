#include "Engine/Geometry/3D/cylinder.h"

#include <glm/ext/scalar_constants.hpp>

namespace Engine {
    Cylinder::Cylinder(GL::Program* program, float radius, float height, int segments) :
        Object(program),
        mRadius(radius),
        mHeight(height),
        mSegments(segments)
    {
        
        float angleStep = 2.0f * glm::pi<float>() / segments;
        for (int i = 0; i <= segments; ++i) {
            float angle = i * angleStep;
            float x = radius * cos(angle);
            float z = radius * sin(angle);

            // Верхняя и нижняя крышки
            mVertices.push_back(glm::vec3(x, height / 2.0f, z));
            mVertices.push_back(glm::vec3(x, -height / 2.0f, z));

            mColors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
            mColors.push_back(glm::vec3(0.0f, 1.0f, 0.0f));

        }

        for (int i = 0; i < segments; ++i) {
            mIndices.push_back(i * 2);
            mIndices.push_back(i * 2 + 1);
            mIndices.push_back((i * 2 + 2) % (segments * 2));

            mIndices.push_back(i * 2 + 1);
            mIndices.push_back((i * 2 + 3) % (segments * 2));
            mIndices.push_back((i * 2 + 2) % (segments * 2));
        }

        mVAO.addVertexBufferObject(mVertices);
        mVAO.addVertexBufferObject(mColors);
        mVAO.addIndices(mIndices);  

    }

    Cylinder::~Cylinder() {
        delete mProgram;
    }

    void Cylinder::draw() {
        mVAO.draw(GL_TRIANGLES);
    }

    void Cylinder::move(const glm::vec3& translation) {
        mTranslationMatrix[3][0] += translation[0];
        mTranslationMatrix[3][1] += translation[1];
        mTranslationMatrix[3][2] += translation[2];
    }

    void Cylinder::rotate(const glm::vec3& angles) {
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

        mRotationMatrix *= rotZ * rotY * rotX;
    }

    void Cylinder::scale(const float& scale) {
        mScaleMatrix[0][0] *= scale;
        mScaleMatrix[1][1] *= scale;
        mScaleMatrix[2][2] *= scale;
    }

    void Cylinder::scale(const glm::vec3& scale) {
        mScaleMatrix[0][0] *= scale[0];
        mScaleMatrix[1][1] *= scale[1];
        mScaleMatrix[2][2] *= scale[2];
    }
}