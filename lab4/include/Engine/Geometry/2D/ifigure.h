#pragma once

namespace Engine {
    class IFigure {
    public:
        virtual ~IFigure() = default;

        virtual void draw() = 0;
    };
}