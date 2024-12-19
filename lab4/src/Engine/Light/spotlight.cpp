#include "Engine/Light/spotlight.h"

namespace Engine {
    // cutoffAngle - внутренний угол конуса света (в градусах), определяет границу яркого света
    // outerCutoffAngle - внешний угол конуса света (в градусах), определяет границу затухания света
    Spotlight::Spotlight(GL::Program* program, glm::vec3 position, glm::vec3 direction, float cutoffAngle, float outerCutoffAngle) :
        mProgram(program), mPosition(position), mDirection(direction) {
            mCutoff = glm::cos(glm::radians(cutoffAngle));
            mOuterCutoff = glm::cos(glm::radians(outerCutoffAngle));
        }

    void Spotlight::setPosition(glm::vec3 position) {
        mPosition = position;
    }

    void Spotlight::setDirection(glm::vec3 direction) {
        mDirection = direction;
    }

    void Spotlight::setUniforms(GL::Program* program) {
        program->setUniform("spotlightPosition", mPosition);
        program->setUniform("spotlightDirection", mDirection);
        program->setUniform("spotlightCutoff", mCutoff);
        program->setUniform("spotlightOuterCutoff", mOuterCutoff);
    }

    void Spotlight::changeAngle(float delta) {
        float cutoffAngle = glm::degrees(glm::acos(mCutoff));
        float outerCutoffAngle = glm::degrees(glm::acos(mOuterCutoff));

        cutoffAngle += delta;
        outerCutoffAngle += delta;

        mCutoff = glm::cos(glm::radians(cutoffAngle));
        mOuterCutoff = glm::cos(glm::radians(outerCutoffAngle));
        // mCutoff = glm::cos(glm::radians(mCutoff + delta));
        // mOuterCutoff = glm::cos(glm::radians(mOuterCutoff + delta));
    }

    void Spotlight::changeDirection(glm::vec3 delta) {
        mDirection = glm::normalize(mDirection + delta);
    }
}