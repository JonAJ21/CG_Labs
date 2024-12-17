#pragma once

#include "Engine/Control/base_controller.h"
#include "GL/program.h"
#include "Engine/Geometry/3D/object.h"
#include "Engine/Camera/camera.h"
namespace Engine {
    class Scene {
    public:
        int mWidth;
        int mHeight;

        BaseController* mController;
        std::vector<Object*> mObjects;
        Object* mActiveObject;

        Camera* mCamera;

        Scene(int width, int height);
        ~Scene();

        void update();
    };
}