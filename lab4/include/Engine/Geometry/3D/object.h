#pragma once

#include <vector>


#include "GL/program.h"
#include "GL/vao.h"

namespace Engine {
    class Object {
    protected:
        GL::VAO mVAO;

        std::vector<glm::vec3> mVertices;
        std::vector<glm::vec3> mNormals;
        std::vector<glm::vec3> mColors;
        std::vector<unsigned int> mIndices; 

        glm::mat4 mTranslationMatrix;
        glm::mat4 mRotationMatrix;
        glm::mat4 mScaleMatrix;
        
    public:
        GL::Program* mProgram;

        Object(GL::Program* program) : 
            mProgram(program), 
            mTranslationMatrix(glm::mat4(1.0f)), 
            mRotationMatrix(glm::mat4(1.0f)), 
            mScaleMatrix(glm::mat4(1.0f)) 
        {};
        virtual ~Object() { delete mProgram;};

        glm::mat4 getModelMatrix() const {
            return mTranslationMatrix * mRotationMatrix * mScaleMatrix;
        };

        virtual void draw() = 0;

        virtual void move(const glm::vec3& translation) = 0;
        virtual void rotate(const glm::vec3& angles) = 0;
        virtual void scale(const glm::vec3& scale) = 0;
        virtual void scale(const float& scale) = 0;
    };
}