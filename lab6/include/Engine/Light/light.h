#pragma once

#include "GL/program.h"
#include "glm/glm.hpp"

namespace Engine {
    class Light {
    public:
        GL::Program* mProgram;

        Light(GL::Program* program) : mProgram(program) {}
        virtual ~Light() {};

        virtual void setPosition(glm::vec3 position) = 0;
        virtual void setDirection(glm::vec3 direction) = 0;

        virtual void setUniforms(GL::Program* program) = 0;
        virtual void setUniforms(GL::Program* program, int index) = 0;

        virtual void changeAngle(float delta) = 0;
        virtual void changeDirection(glm::vec3 delta) = 0;
    };
}