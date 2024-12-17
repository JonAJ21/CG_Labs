#include "Engine/Control/controller.h"
#include <iostream>

namespace Engine {

    Controller::Controller(Engine::Scene* scene) : BaseController(scene) {
    }

    Controller::~Controller() {
    }

    void Controller::pressW() {
        mScene->mActivePolygon->move({ 0.0f, 0.001f, 0.0f });
    }

    void Controller::pressA() {
        mScene->mActivePolygon->move({ -0.001f, 0.0f, 0.0f });
    }

    void Controller::pressS() {
        mScene->mActivePolygon->move({ 0.0f, -0.001f, 0.0f });
    }

    void Controller::pressD() {
        mScene->mActivePolygon->move({ 0.001f, 0.0f, 0.0f });
    }

    void Controller::pressE() {
        mScene->mActivePolygon->rotate({ 0.0f, 0.0f, 0.1f });
    }

    void Controller::pressQ() {
        mScene->mActivePolygon->rotate({ 0.0f, 0.0f, -0.1f });
    }
    
    void Controller::pressLeftShift() {
        mScene->mActivePolygon->scale(1.001f);
    }

    void Controller::pressLeftControl() {
        mScene->mActivePolygon->scale(0.999f);
    }
}