#pragma once

#include <SFML/Window.hpp>

#include "Engine/Scene/scene.h"


class Window {
private:
    sf::Window* mWindow;
    Engine::Scene* mScene;
    int mFps;

    void loop();

public:
    Window(const std::string &title, int width, int height, int fps);
    ~Window();

    void processInput(sf::Window* window);
};