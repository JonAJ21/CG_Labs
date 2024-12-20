#include "Engine/Geometry/3D/pyramid.h"

#include <glm/ext/scalar_constants.hpp>
#include <iostream>

namespace Engine {

    Pyramid::Pyramid(std::vector<GL::Program*>& programs) : Object(programs) {
        // Вершины и нормали пирамиды
        mVertices = {
            // Основание
            glm::vec3(-0.5f, 0.0f, -0.5f),
            glm::vec3(0.5f, 0.0f, -0.5f),
            
            glm::vec3(0.5f, 0.0f, 0.5f),
            glm::vec3(-0.5f, 0.0f, 0.5f),
            
            // Передняя грань
            glm::vec3(-0.5f, 0.0f, 0.5f),
            glm::vec3(0.5f, 0.0f, 0.5f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            
            // Правая грань
            glm::vec3(0.5f, 0.0f, 0.5f),
            glm::vec3(0.5f, 0.0f, -0.5f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            
            // Задняя грань
            glm::vec3(0.5f, 0.0f, -0.5f),
            glm::vec3(-0.5f, 0.0f, -0.5f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            
            // Левая грань
            glm::vec3(-0.5f, 0.0f, -0.5f),
            glm::vec3(-0.5f, 0.0f, 0.5f),
            glm::vec3(0.0f, 1.0f, 0.0f)
        };

        mNormals = {
            // Основание
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            
            // Передняя грань
            glm::vec3(0.0f, 0.5f, 0.5f),
            glm::vec3(0.0f, 0.5f, 0.5f),
            glm::vec3(0.0f, 0.5f, 0.5f),
            
            // Правая грань
            glm::vec3(0.5f, 0.5f, 0.0f),
            glm::vec3(0.5f, 0.5f, 0.0f),
            glm::vec3(0.5f, 0.5f, 0.0f),
            
            // Задняя грань
            glm::vec3(0.0f, 0.5f, -0.5f),
            glm::vec3(0.0f, 0.5f, -0.5f),
            glm::vec3(0.0f, 0.5f, -0.5f),
            
            // Левая грань
            glm::vec3(-0.5f, 0.5f, 0.0f),
            glm::vec3(-0.5f, 0.5f, 0.0f),
            glm::vec3(-0.5f, 0.5f, 0.0f)
        };
        
        mIndices = {
            // Основание
            0, 1, 2,
            0, 2, 3,
            
            // Передняя грань
            4, 5, 6,
            
            // Правая грань 
            7, 8, 9,
            
            // Задняя грань
            10, 11, 12,
            
            // Левая грань
            13, 14, 15
        };

        // mIndices = {
        //     0, 1, 2,
        //     0, 2, 3,
        //     0, 4, 1,
        //     1, 4, 2,
        //     2, 4, 3,
        //     3, 4, 0
        // };
 
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