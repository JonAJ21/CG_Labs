#include <stdexcept>

#include "Engine/Geometry/2D/polygon.h"
#include "Engine/Scene/scene.h"
#include "Engine/Control/controller.h"
#include "GL/gl.h"
#include "GL/program.h"

namespace Engine {

    Scene::Scene() {
        // path from build folder
        GL::Program* first = new GL::Program("../res/glsl/shader");
        first->bindAttribute(0, "position");
        first->bindAttribute(1, "color");
        first->link();
        first->use();

        Engine::Polygon* polygon = new Engine::Polygon(
        {
            {-0.3f, -0.5f, 0.0f},
            {0.3f, -0.5f, 0.0f},
            {0.5f, 0.0f, 0.0f},
            {0.0f, 0.5f, 0.0f},
            {-0.5f, 0.0f, 0.0f}
        },
        {
            {1.0f, 0.0f, 0.0f}, // Red
            {1.0f, 0.0f, 0.0f}, // Red
            {0.0f, 1.0f, 0.0f}, // Green
            {0.0f, 0.0f, 1.0f}, // Blue
            {0.0f, 1.0f, 0.0f}  // Green
        }, 
            {0, 1, 2, 3, 4},
            first
        );

        mPolygons.push_back(polygon);

        // mController = new Controller();
        // if (!mController) {
        //     throw std::runtime_error("Failed to create controller");
        // }
        mController = new Controller(this);
        if (!mController) {
            throw std::runtime_error("Failed to create controller");
        }

        mActivePolygon = mPolygons[0];
    }

    Scene::~Scene() {
        delete mController;
        for (auto polygon : mPolygons) {
            delete polygon;
        }
    }

    void Scene::update() {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        for (auto polygon : mPolygons) {
            polygon->mProgram->use();
            polygon->mProgram->setUniform("modelMatrix", polygon->getModelMatrix());
            polygon->draw();
            
        }

    }
}