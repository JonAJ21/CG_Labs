#pragma once

#include "Engine/Control/base_controller.h"
#include "Engine/Scene/scene.h"

namespace Engine {
    class Controller : public BaseController {
    public:
        Controller(Scene* scene);

        ~Controller();

        void pressW() override;
        void pressA() override;
        void pressS() override;
        void pressD() override;

        void pressE() override;
        void pressQ() override;

        void pressLeftShift() override;
        void pressLeftControl() override;

        void pressUp() override;
        void pressDown() override;
        void pressLeft() override;
        void pressRight() override;

        void mouseScroll(int y) override;

        void mouseMove(int x, int y) override;
    };
}