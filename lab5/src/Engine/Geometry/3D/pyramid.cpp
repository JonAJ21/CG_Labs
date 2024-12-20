#include "Engine/Geometry/3D/pyramid.h"

#include <glm/ext/scalar_constants.hpp>
#include <iostream>

namespace Engine {

    Pyramid::Pyramid(std::vector<GL::Program*>& programs) : Object(programs) {
        // Вершины пирамиды
        mVertices = {
            // Основание (квадрат)
            glm::vec3(-0.5f, -0.5f, -0.5f),  // 0
            glm::vec3(0.5f, -0.5f, -0.5f),   // 1
            glm::vec3(0.5f, -0.5f, 0.5f),    // 2
            glm::vec3(-0.5f, -0.5f, 0.5f),   // 3
            // Вершина пирамиды
            glm::vec3(0.0f, 0.5f, 0.0f)      // 4
        };

        // Цвета для вершин
        mColors = {
            glm::vec3(1.0f, 0.0f, 0.0f),  // Красный
            glm::vec3(0.0f, 1.0f, 0.0f),  // Зеленый
            glm::vec3(0.0f, 0.0f, 1.0f),  // Синий
            glm::vec3(1.0f, 1.0f, 0.0f),  // Желтый
            glm::vec3(1.0f, 0.0f, 1.0f)   // Пурпурный
        };

        // Индексы для треугольников
        mIndices = {
            // Основание
            0, 1, 2,  // Первый треугольник основания
            0, 2, 3,  // Второй треугольник основания
            // Боковые грани
            0, 4, 1,  // Передняя грань
            1, 4, 2,  // Правая грань
            2, 4, 3,  // Задняя грань
            3, 4, 0   // Левая грань
        };

        // Вычисление нормалей для каждой грани
        glm::vec3 normalBase = glm::vec3(0.0f, -1.0f, 0.0f);  // Нормаль основания
        glm::vec3 normalFront = glm::normalize(glm::cross(mVertices[1] - mVertices[0], mVertices[4] - mVertices[0]));
        glm::vec3 normalRight = glm::normalize(glm::cross(mVertices[2] - mVertices[1], mVertices[4] - mVertices[1]));
        glm::vec3 normalBack = glm::normalize(glm::cross(mVertices[3] - mVertices[2], mVertices[4] - mVertices[2]));
        glm::vec3 normalLeft = glm::normalize(glm::cross(mVertices[0] - mVertices[3], mVertices[4] - mVertices[3]));

        // Нормали для каждой вершины
        mNormals = {
            normalBase,  // Вершина 0 (основание)
            normalBase,  // Вершина 1 (основание) 
            normalBase,  // Вершина 2 (основание)
            normalBase,  // Вершина 3 (основание)
            (normalFront + normalRight + normalBack + normalLeft) / 4.0f  // Вершина 4 (вершина пирамиды)
        };


        mIndices = {
            0, 1, 2,
            0, 2, 3,
            0, 4, 1,
            1, 4, 2,
            2, 4, 3,
            3, 4, 0
        };
 
        mVAO.addVertexBufferObject(mVertices);
        mVAO.addVertexBufferObject(mColors);
        mVAO.addVertexBufferObject(mNormals);
        mVAO.addIndices(mIndices);

    }

    Pyramid::~Pyramid() {
        // delete mProgram;
        for (auto program : mPrograms) {
            delete program;
        }
    }

    void Pyramid::draw() {
        mVAO.draw(GL_TRIANGLES);
    }

    void Pyramid::move(const glm::vec3& translation) {
        mTranslationMatrix[3][0] += translation[0];
        mTranslationMatrix[3][1] += translation[1];
        mTranslationMatrix[3][2] += translation[2];
    }

    void Pyramid::rotate(const glm::vec3& angles) {
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

    void Pyramid::scale(const float& scale) {
        mScaleMatrix[0][0] *= scale;
        mScaleMatrix[1][1] *= scale;
        mScaleMatrix[2][2] *= scale;
    }

    void Pyramid::scale(const glm::vec3& scale) {
        mScaleMatrix[0][0] *= scale[0];
        mScaleMatrix[1][1] *= scale[1];
        mScaleMatrix[2][2] *= scale[2];
    }
    

}