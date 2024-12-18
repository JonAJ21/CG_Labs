#include "Engine/Geometry/3D/sphere.h"

#include <glm/ext/scalar_constants.hpp>

namespace Engine {
    Sphere::Sphere(GL::Program* program, float radius, int segments) : Object(program) {
        mRadius = radius;
        mSegments = segments;

        // Вычисляем шаги для углов
        float phiStep = 2.0f * glm::pi<float>() / segments;
        float thetaStep = glm::pi<float>() / segments;

        // Генерируем вершины сферы
        for (int i = 0; i <= segments; ++i) {
            float theta = i * thetaStep;
            float sinTheta = sin(theta);
            float cosTheta = cos(theta);

            for (int j = 0; j <= segments; ++j) {
                float phi = j * phiStep;
                float sinPhi = sin(phi);
                float cosPhi = cos(phi);

                // Вычисляем координаты вершины
                float x = radius * sinTheta * cosPhi;
                float y = radius * cosTheta;
                float z = radius * sinTheta * sinPhi;

                // Добавляем вершину
                glm::vec3 vertex(x, y, z);
                mVertices.push_back(vertex);

                // Нормаль для сферы - это нормализованный вектор от центра к вершине
                glm::vec3 normal = glm::normalize(vertex);
                mNormals.push_back(normal);

                // Добавляем цвет (градиент от красного к зеленому)
                float t = (float)i / segments;
                mColors.push_back(glm::vec3(1.0f - t, t, 0.0f));
            }
        }

        // Создаем индексы для треугольников
        for (int i = 0; i < segments; ++i) {
            for (int j = 0; j < segments; ++j) {
                int first = i * (segments + 1) + j;
                int second = first + segments + 1;

                // Первый треугольник
                mIndices.push_back(first);
                mIndices.push_back(second);
                mIndices.push_back(first + 1);

                // Второй треугольник
                mIndices.push_back(second);
                mIndices.push_back(second + 1);
                mIndices.push_back(first + 1);
            }
        }

        mVAO.addVertexBufferObject(mVertices);
        mVAO.addVertexBufferObject(mColors);
        mVAO.addVertexBufferObject(mNormals);
        mVAO.addIndices(mIndices);
    }

    Sphere::~Sphere() {
        delete mProgram;
    }

    void Sphere::draw() {
        mVAO.draw(GL_TRIANGLES);
    }

    void Sphere::move(const glm::vec3& translation) {
        mTranslationMatrix[3][0] += translation[0];
        mTranslationMatrix[3][1] += translation[1];
        mTranslationMatrix[3][2] += translation[2];
    }

    void Sphere::rotate(const glm::vec3& angles) {
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
    
    void Sphere::scale(const float& scale) {
        mScaleMatrix[0][0] *= scale;
        mScaleMatrix[1][1] *= scale;
        mScaleMatrix[2][2] *= scale;
    }

    void Sphere::scale(const glm::vec3& scale) {
        mScaleMatrix[0][0] *= scale[0];
        mScaleMatrix[1][1] *= scale[1];
        mScaleMatrix[2][2] *= scale[2];
    }

}   