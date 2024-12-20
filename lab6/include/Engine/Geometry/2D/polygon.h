#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "Engine/Geometry/2D/ifigure.h"
#include "GL/program.h"
#include "GL/vao.h"

namespace Engine {
    class Polygon : public IFigure {
    private:
        GL::VAO mVAO;

        std::vector<glm::vec3> mVertices;
        std::vector<glm::vec3> mColors;
        std::vector<unsigned int> mIndices;

        glm::mat4 mTranslationMatrix;
        glm::mat4 mRotationMatrix;
        glm::mat4 mScaleMatrix;
        
    public:
        GL::Program* mProgram;

        Polygon(const std::vector<glm::vec3>& vertices,
                const std::vector<glm::vec3>& colors,
                const std::vector<unsigned int>& indices,
                GL::Program* program);
        
        ~Polygon();


        glm::mat4 getModelMatrix() const;

        void draw() override;

        void move(const glm::vec3& translation);
        void rotate(const glm::vec3& angles);
        void scale(const glm::vec3& scale);
        void scale(const float& scale);

    };
}