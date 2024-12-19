#pragma once

#include "Engine/Geometry/3D/object.h"

namespace Engine {

    class Pyramid : public Object {
    public:
        Pyramid(GL::Program* program);
        ~Pyramid();

        void draw() override;

        void move(const glm::vec3& translation) override;
        void rotate(const glm::vec3& angles) override;
        void scale(const glm::vec3& scale) override;
        void scale(const float& scale) override;
    };
}