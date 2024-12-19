#pragma once

#include "glm/glm.hpp"

#include "GL/program.h"

namespace Engine {
    class Spotlight {
    public:
        glm::vec3 mPosition;
        glm::vec3 mDirection;
        float mCutoff;
        float mOuterCutoff;
        
        GL::Program* mProgram;

        Spotlight(GL::Program* program, glm::vec3 position, glm::vec3 direction, float cutoffAngle, float outerCutoffAngle);
        ~Spotlight();

        void setPosition(glm::vec3 position);
        void setDirection(glm::vec3 direction);
        
        void setUniforms(GL::Program* program);

        void changeAngle(float angle);
        void changeDirection(glm::vec3 direction);

    };
}