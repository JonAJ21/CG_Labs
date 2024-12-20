#pragma once

#include "Engine/Control/base_controller.h"
#include "GL/program.h"
#include "Engine/Geometry/3D/object.h"
#include "Engine/Camera/camera.h"
#include "Engine/Light/spotlight.h"

namespace Engine {
    class Scene {
    public:
        int mWidth;
        int mHeight;

        BaseController* mController;
        // Spotlight* mSpotlight;

        std::vector<Light*> mLights;
        Light* mActiveLight;
        
        std::vector<Object*> mObjects;
        Object* mActiveObject;

        Camera* mCamera;

        Scene(int width, int height);
        ~Scene();

        void update();
    };
}