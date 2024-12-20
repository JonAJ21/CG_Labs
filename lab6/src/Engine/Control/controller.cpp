#include "Engine/Control/controller.h"
#include <iostream>

namespace Engine {

    Controller::Controller(Engine::Scene* scene) : BaseController(scene) {
    }

    Controller::~Controller() {
    }

    void Controller::pressW() {
        mScene->mActiveObject->move({ 0.0f, 0.001f, 0.0f });
    }

    void Controller::pressA() {
        mScene->mActiveObject->move({ -0.001f, 0.0f, 0.0f });
    }

    void Controller::pressS() {
        mScene->mActiveObject->move({ 0.0f, -0.001f, 0.0f });
    }

    void Controller::pressD() {
        mScene->mActiveObject->move({ 0.001f, 0.0f, 0.0f });
    }

    void Controller::pressE() {
        mScene->mActiveObject->rotate({ 0.0f, 0.0f, 0.1f });
    }

    void Controller::pressQ() {
        mScene->mActiveObject->rotate({ 0.0f, 0.0f, -0.1f });
    }
    
    void Controller::pressLeftShift() {
        mScene->mActiveObject->scale(1.001f);
    }

    void Controller::pressLeftControl() {
        mScene->mActiveObject->scale(0.999f);
    }

    void Controller::pressUp() {
        mScene->mCamera->ProcessKeyboard(CameraMovement::FORWARD, 0.001f);
    }

    void Controller::pressDown() {
        mScene->mCamera->ProcessKeyboard(CameraMovement::BACKWARD, 0.001f);
    }

    void Controller::pressLeft() {
        mScene->mCamera->ProcessKeyboard(CameraMovement::LEFT, 0.001f);
    }

    void Controller::pressRight() {
        mScene->mCamera->ProcessKeyboard(CameraMovement::RIGHT, 0.001f);
    }

    void Controller::pressSpace() {
        mScene->mCamera->ProcessKeyboard(CameraMovement::UP, 0.001f);
    }

    void Controller::pressLeftAlt() {
        mScene->mCamera->ProcessKeyboard(CameraMovement::DOWN, 0.001f);
    }

    void Controller::pressXPlus() {
        mScene->mCamera->ProccessMouseMovementAroundCenter(100.0f, 0.0f);
    }

    void Controller::pressYPlus() {
        mScene->mCamera->ProccessMouseMovementAroundCenter(0.0f, 100.0f);
    }   

    void Controller::pressZPlus() {
        mScene->mCamera->ProccessMouseMovementAroundCenter(100.0f, 100.0f);
    }

    void Controller::pressXMinus() {
        mScene->mCamera->ProccessMouseMovementAroundCenter(-100.0f, 0.0f);
    }

    void Controller::pressYMinus() {
        mScene->mCamera->ProccessMouseMovementAroundCenter(0.0f, -10.0f);
    }   

    void Controller::pressZMinus() {
        mScene->mCamera->ProccessMouseMovementAroundCenter(-100.0f, -100.0f);
    }

    void Controller::mouseScroll(int y) {
        mScene->mCamera->ProcessMouseScroll(y * 5.0f);
    }

    void Controller::mouseMove(int x, int y) {
        GLfloat xOffset = x - mScene->mWidth / 2;
        GLfloat yOffset = mScene->mHeight / 2 - y;
        //std::cout << "xoffset: " << xOffset << " yoffset: " << yOffset << std::endl;
        mScene->mCamera->ProcessMouseMovement(xOffset, yOffset);
        // mScene->mCamera->ProccessMouseMovementAroundCenter(xOffset, yOffset);
    }

    void Controller::mouseScrollC(int y) {
        // std::cout << "y: " << y << std::endl;
        // std::cout << "cutoff: " << mScene->mActiveLight->mCutoff << std::endl;
        // std::cout << "outer cutoff: " << mScene->mActiveLight->mOuterCutoff << std::endl;

        // float cutoffAngle = glm::acos(mScene->mActiveLight->mCutoff);
        // float outerCutoffAngle = glm::acos(mScene->mActiveLight->mOuterCutoff);
        // std::cout << "cutoff angle: " << cutoffAngle << std::endl;
        // std::cout << "outer cutoff angle: " << outerCutoffAngle << std::endl;
        mScene->mActiveLight->changeAngle(y * 5.0f);
    }

    void Controller::pressI() {
        mScene->mActiveLight->changeDirection({ 0.0f, 0.001f, 0.0f });
    }

    void Controller::pressJ() {
        mScene->mActiveLight->changeDirection({ -0.001f, 0.0f, -0.001f });
    } 

    void Controller::pressK() {
        mScene->mActiveLight->changeDirection({ 0.0f, -0.001f, 0.0f });
    }

    void Controller::pressL() {
        mScene->mActiveLight->changeDirection({ 0.001f, 0.000f, 0.0f });
    }
}

