#include <stdexcept>

#include "Engine/Scene/scene.h"
#include "Engine/Control/controller.h"
#include "GL/gl.h"
#include "GL/program.h"
#include "Engine/Geometry/3D/object.h"
#include "Engine/Geometry/3D/cylinder.h"
#include "Engine/Geometry/3D/sphere.h"
#include "Engine/Geometry/3D/pyramid.h"
#include "Engine/Light/spotlight.h"

namespace Engine {

    Scene::Scene(int width, int height) : mWidth(width), mHeight(height) {

        mCamera = new Camera(glm::vec3(0.0f, 0.0f, 2.0f));
        if (!mCamera) {
            throw std::runtime_error("Failed to create camera");
        }

        // path from build folder
        GL::Program* first = new GL::Program("../res/glsl/shader");
        first->bindAttribute(0, "position");
        first->bindAttribute(1, "color");
        first->bindAttribute(2, "normal");
        first->link();
        first->use();


        Engine::Object* pyramid = new Engine::Pyramid(first);
        if (!pyramid) {
            throw std::runtime_error("Failed to create pyramid");
        }

        mObjects.push_back(pyramid);
        
        // Engine::Object* sphere = new Engine::Sphere(first, 0.3f, 32);
        // if (!sphere) {
        //     throw std::runtime_error("Failed to create sphere");
        // }
        // mObjects.push_back(sphere);

        mController = new Controller(this);
        if (!mController) {
            throw std::runtime_error("Failed to create controller");
        }

        mActiveObject = mObjects[0];

    

        mSpotlight = new Spotlight(
            first,
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec3(0.0f, 0.0f, -1.0f),
            17.5f, 20.5f);
        if (!mSpotlight) {
            throw std::runtime_error("Failed to create spotlight");
        }


    }

    Scene::~Scene() {
        delete mCamera;
        delete mController;
        for (auto object : mObjects) {
            delete object;
        }
    }

    void Scene::update() {
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = mCamera->getViewMatrix();
        glm::mat4 projection = mCamera->getProjectionMatrix();

        mSpotlight->setUniforms(mSpotlight->mProgram);

        for (auto object : mObjects) {
            object->mProgram->use();
            object->mProgram->setUniform("modelMatrix", object->getModelMatrix());
            object->mProgram->setUniform("viewMatrix", view);
            object->mProgram->setUniform("projectionMatrix", projection);
            object->draw();
        }

    }
}