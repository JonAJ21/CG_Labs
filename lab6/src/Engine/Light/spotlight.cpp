#include "Engine/Light/spotlight.h"

namespace Engine {
    
    // Spotlight::Spotlight(GL::Program* program) : Light(program) {}
    
    Spotlight::Spotlight(GL::Program* program, glm::vec3 position, glm::vec3 direction, float cutoffAngle, float outerCutoffAngle) :
        Light(program), mPosition(position), mDirection(direction) {
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
        program->setUniform("fSpotlightPosition[0]", mPosition);
        program->setUniform("fSpotlightDirection[0]", mDirection);
        program->setUniform("fSpotlightCutoff[0]", mCutoff);
        program->setUniform("fSpotlightOuterCutoff[0]", mOuterCutoff);
    }

    void Spotlight::setUniforms(GL::Program* program, int index) {
        program->setUniform("fSpotlightPosition[" + std::to_string(index) + "]", mPosition);
        program->setUniform("fSpotlightDirection[" + std::to_string(index) + "]", mDirection);
        program->setUniform("fSpotlightCutoff[" + std::to_string(index) + "]", mCutoff);
        program->setUniform("fSpotlightOuterCutoff[" + std::to_string(index) + "]", mOuterCutoff);
    }

    void Spotlight::changeAngle(float delta) {
        float cutoffAngle = glm::degrees(glm::acos(mCutoff));
        float outerCutoffAngle = glm::degrees(glm::acos(mOuterCutoff));

        cutoffAngle += delta;
        outerCutoffAngle += delta;

        mCutoff = glm::cos(glm::radians(cutoffAngle));
        mOuterCutoff = glm::cos(glm::radians(outerCutoffAngle));
    }

    void Spotlight::changeDirection(glm::vec3 delta) {
        mDirection = glm::normalize(mDirection + delta);
    }
}