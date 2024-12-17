#include "SFML/window.h"

#include "GL/gl.h"
#include "Engine/Scene/scene.h"

Window::Window(const std::string &title, int width, int height, int fps) : mFps(fps) {
    
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 3;
    settings.minorVersion = 3;
    
    sf::Window* window = new sf::Window(sf::VideoMode(width, height), title, sf::Style::Default, settings);
    if (!window->isOpen()) {
        throw std::runtime_error("Failed to open window");
    }

    mWindow = window;

    mWindow->setVerticalSyncEnabled(false);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        throw std::runtime_error("Failed to initialize GLEW");
    }

    mScene = new Engine::Scene(width, height);
    if (!mScene) {
        throw std::runtime_error("Failed to create scene");
    }

    loop();
}

Window::~Window() {
    delete mWindow;
}

void Window::processInput(sf::Window* window) {
    sf::Event event;
    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window->close();  
    }
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        mScene->mController->pressW();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        mScene->mController->pressA();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        mScene->mController->pressS();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        mScene->mController->pressD();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
        mScene->mController->pressE();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
        mScene->mController->pressQ();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
        mScene->mController->pressLeftShift();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
        mScene->mController->pressLeftControl();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        mScene->mController->pressUp();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        mScene->mController->pressDown();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        mScene->mController->pressLeft();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        mScene->mController->pressRight();
    }

    if (event.type == sf::Event::MouseWheelScrolled) {
        if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
            mScene->mController->mouseScroll(event.mouseWheelScroll.delta);
        }
    }

    if (event.type == sf::Event::MouseMoved && sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
        mScene->mController->mouseMove(event.mouseMove.x, event.mouseMove.y);
    }
}

void Window::loop() {
    sf::Clock clock;
    while (mWindow->isOpen()) {
        processInput(mWindow);

        mScene->update();

        mWindow->display();

        sf::Int32 time = clock.getElapsedTime().asMilliseconds();
        if (time < 1000 / mFps) {
            sf::sleep(sf::milliseconds(1000 / mFps - time));
        }
    }
}