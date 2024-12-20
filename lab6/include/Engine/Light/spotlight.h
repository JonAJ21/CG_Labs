#pragma once

#include "glm/glm.hpp"

#include "GL/program.h"
#include "Engine/Light/light.h"

namespace Engine {
    class Spotlight : public Light {
    public:
        glm::vec3 mPosition;
        glm::vec3 mDirection;
        float mCutoff;
        float mOuterCutoff;
        
        GL::Program* mProgram;

        // Spotlight(GL::Program* program);
        Spotlight(GL::Program* program, glm::vec3 position, glm::vec3 direction, float cutoffAngle, float outerCutoffAngle);
        ~Spotlight() {};

        void setPosition(glm::vec3 position) override;
        void setDirection(glm::vec3 direction) override;
        
        void setUniforms(GL::Program* program) override;
        void setUniforms(GL::Program* program, int index) override;

        void changeAngle(float delta) override;
        void changeDirection(glm::vec3 delta) override;

    };
}