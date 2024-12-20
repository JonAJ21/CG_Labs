#pragma once

#include "Engine/Geometry/3D/object.h"

namespace Engine {
    class Cylinder : public Object {
    private:
        float mRadius;
        float mHeight;
        int mSegments;

    public:
        Cylinder(std::vector<GL::Program*>& programs, float radius, float height, int segments);
        ~Cylinder();

        void draw() override;

        void move(const glm::vec3& translation) override;
        void rotate(const glm::vec3& angles) override;
        void scale(const glm::vec3& scale) override;
        void scale(const float& scale) override;
    };
}