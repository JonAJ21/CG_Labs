#pragma once

#include "Engine/Control/base_controller.h"
#include "GL/program.h"
#include "Engine/Geometry/2D/polygon.h"
namespace Engine {
    class Scene {
    public:
        BaseController* mController;
        std::vector<Polygon*> mPolygons;
        Polygon* mActivePolygon;

        Scene();
        ~Scene();

        void update();
    };
}