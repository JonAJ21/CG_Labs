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
#include "Engine/Light/light.h"
namespace Engine {

    Scene::Scene(int width, int height) : mWidth(width), mHeight(height) {
        glEnable(GL_DEPTH_TEST);
        // glEnable(GL_CULL_FACE);
        // glCullFace(GL_BACK);

        mCamera = new Camera(glm::vec3(0.0f, 0.0f, 2.0f));
        if (!mCamera) {
            throw std::runtime_error("Failed to create camera");
        }

        std::vector<GL::Program*> programs;
        // path from build folder
        GL::Program* first = new GL::Program("../res/glsl/shader");
        first->bindAttribute(0, "position");
        first->bindAttribute(1, "color");
        first->bindAttribute(2, "normal");
        first->link();
        first->use();

        programs.push_back(first);

        
        Engine::Object* pyramid = new Engine::Pyramid(programs);
        if (!pyramid) {
            throw std::runtime_error("Failed to create pyramid");
        }

        mObjects.push_back(pyramid);
        
        // Engine::Object* sphere = new Engine::Sphere(programs, 0.3f, 32);
        // if (!sphere) {
        //     throw std::runtime_error("Failed to create sphere");
        // }
        // mObjects.push_back(sphere);
        // sphere->move(glm::vec3(-0.5f, 0.0f, 0.0f));

        // sphere = new Engine::Sphere(programs, 0.3f, 32);
        // if (!sphere) {
        //     throw std::runtime_error("Failed to create sphere");
        // }
        // mObjects.push_back(sphere);
        // sphere->move(glm::vec3(0.5f, 0.0f, 0.0f));
        

        mController = new Controller(this);
        if (!mController) {
            throw std::runtime_error("Failed to create controller");
        }
        Light* light = new Spotlight(
            first,
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec3(0.0f, 0.0f, -1.0f),
            17.5f, 20.5f);
        if (!light) {
            throw std::runtime_error("Failed to create spotlight");
        }

        mLights.push_back(light);

        // Light* light2 = new Spotlight(
        //     first,
        //     glm::vec3(0.0f, -1.0f, 0.0f),
        //     glm::vec3(0.0f, 1.0f, 0.0f),
        //     17.5f, 20.5f);
        // if (!light2) {
        //     throw std::runtime_error("Failed to create spotlight");
        // }

        // mLights.push_back(light2);
        
        // Light* light3 = new Spotlight(
        //     first,
        //     glm::vec3(-1.0f, 0.0f, 0.0f),
        //     glm::vec3(1.0f, 0.0f, 0.0f),
        //     17.5f, 20.5f);
        // if (!light3) {
        //     throw std::runtime_error("Failed to create spotlight");
        // }

        // mLights.push_back(light3);

        mActiveObject = mObjects[0];
        mActiveLight = mLights[0];
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

        // mSpotlight->setUniforms(mSpotlight->mProgram);

        int index = 0;
        for (auto light : mLights) {
            light->mProgram->use();
            light->setUniforms(light->mProgram, index);
            index++;
        }

        for (auto object : mObjects) {
            for (auto program : object->mPrograms) {
                program->use();
                program->setUniform("modelMatrix", object->getModelMatrix());
                program->setUniform("viewMatrix", view);
                program->setUniform("projectionMatrix", projection);
                object->draw();
            }
            
            // object->mProgram->use();
            // object->mProgram->setUniform("modelMatrix", object->getModelMatrix());
            // object->mProgram->setUniform("viewMatrix", view);
            // object->mProgram->setUniform("projectionMatrix", projection);
            
            // for (auto light : mLights) {
            //     light->setUniforms(light->mProgram);
            // }

            // object->draw();



        }

    }
}