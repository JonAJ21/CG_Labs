#pragma once
#include <iostream>

namespace Engine {

    class Scene;
    class BaseController {
    public:
        Scene* mScene;
        
        BaseController() {};
        BaseController(Scene* scene) : mScene(scene) {};
        virtual ~BaseController() {};

        virtual void leftClick() {std::cout << "Base left click" << std::endl;};
        virtual void rightClick() {std::cout << "Base right click" << std::endl;};
        virtual void middleClick() {std::cout << "Base middle click" << std::endl;};
        virtual void mouseMove(int x, int y) {std::cout << "Base mouse move" << std::endl;};
        virtual void mouseScroll(int x, int y) {std::cout << "Base mouse scroll" << std::endl;};

        virtual void pressLeftShift() {std::cout << "Base Left Shift pressed" << std::endl;};
        virtual void pressLeftControl() {std::cout << "Base Left Control pressed" << std::endl;};
        virtual void pressLeftAlt() {std::cout << "Base Left Alt pressed" << std::endl;};
        virtual void pressLeftSuper() {std::cout << "Base Left Super pressed" << std::endl;};
        virtual void pressRightShift() {std::cout << "Base Right Shift pressed" << std::endl;};
        virtual void pressRightControl() {std::cout << "Base Right Control pressed" << std::endl;};
        virtual void pressRightAlt() {std::cout << "Base Right Alt pressed" << std::endl;};
        virtual void pressRightSuper() {std::cout << "Base Right Super pressed" << std::endl;};

        virtual void pressEscape() {std::cout << "Base Escape pressed" << std::endl;};
        virtual void pressSpace() {std::cout << "Base Space pressed" << std::endl;};
        virtual void pressEnter() {std::cout << "Base Enter pressed" << std::endl;};
        virtual void pressLeft() {std::cout << "Base Left pressed" << std::endl;};
        virtual void pressRight() {std::cout << "Base Right pressed" << std::endl;};
        virtual void pressUp() {std::cout << "Base Up pressed" << std::endl;};
        virtual void pressDown() {std::cout << "Base Down pressed" << std::endl;};
        virtual void pressW() {std::cout << "Base w pressed" << std::endl;};
        virtual void pressA() {std::cout << "Base a pressed" << std::endl;};
        virtual void pressS() {std::cout << "Base s pressed" << std::endl;};
        virtual void pressD() {std::cout << "Base d pressed" << std::endl;};
        virtual void pressQ() {std::cout << "Base q pressed" << std::endl;};
        virtual void pressE() {std::cout << "Base e pressed" << std::endl;};
        virtual void pressR() {std::cout << "Base r pressed" << std::endl;};
        virtual void pressF() {std::cout << "Base f pressed" << std::endl;};
        virtual void pressG() {std::cout << "Base g pressed" << std::endl;};
        virtual void pressH() {std::cout << "Base h pressed" << std::endl;};
        virtual void pressI() {std::cout << "Base i pressed" << std::endl;};
        virtual void pressJ() {std::cout << "Base j pressed" << std::endl;};
        virtual void pressK() {std::cout << "Base k pressed" << std::endl;};
        virtual void pressL() {std::cout << "Base l pressed" << std::endl;}; 
        virtual void pressZ() {std::cout << "Base z pressed" << std::endl;};
        virtual void pressX() {std::cout << "Base x pressed" << std::endl;};
        virtual void pressC() {std::cout << "Base c pressed" << std::endl;};
        virtual void pressV() {std::cout << "Base v pressed" << std::endl;};
        virtual void pressB() {std::cout << "Base b pressed" << std::endl;};
        virtual void pressN() {std::cout << "Base n pressed" << std::endl;};
        virtual void pressM() {std::cout << "Base m pressed" << std::endl;};
        virtual void press1() {std::cout << "Base 1 pressed" << std::endl;};
        virtual void press2() {std::cout << "Base 2 pressed" << std::endl;};
        virtual void press3() {std::cout << "Base 3 pressed" << std::endl;};
        virtual void press4() {std::cout << "Base 4 pressed" << std::endl;};
        virtual void press5() {std::cout << "Base 5 pressed" << std::endl;};
        virtual void press6() {std::cout << "Base 6 pressed" << std::endl;};
        virtual void press7() {std::cout << "Base 7 pressed" << std::endl;};
        virtual void press8() {std::cout << "Base 8 pressed" << std::endl;};
        virtual void press9() {std::cout << "Base 9 pressed" << std::endl;};
        virtual void press0() {std::cout << "Base 0 pressed" << std::endl;};
        virtual void pressMinus() {std::cout << "Base - pressed" << std::endl;};
        virtual void pressEqual() {std::cout << "Base = pressed" << std::endl;};
        virtual void pressBackslash() {std::cout << "Base \\ pressed" << std::endl;};
        virtual void pressSemicolon() {std::cout << "Base ; pressed" << std::endl;};
        virtual void pressQuote() {std::cout << "Base ' pressed" << std::endl;};
        virtual void pressComma() {std::cout << "Base , pressed" << std::endl;};
        virtual void pressPeriod() {std::cout << "Base . pressed" << std::endl;};
        virtual void pressSlash() {std::cout << "Base / pressed" << std::endl;};
        virtual void pressBacktick() {std::cout << "Base ` pressed" << std::endl;};
        virtual void pressLeftBracket() {std::cout << "Base [ pressed" << std::endl;};
        virtual void pressRightBracket() {std::cout << "Base ] pressed" << std::endl;};
        virtual void pressLeftBrace() {std::cout << "Base { pressed" << std::endl;};
        virtual void pressRightBrace() {std::cout << "Base } pressed" << std::endl;};
        virtual void pressPipe() {std::cout << "Base | pressed" << std::endl;};
        virtual void pressTilde() {std::cout << "Base ~ pressed" << std::endl;};
        virtual void pressNumlock() {std::cout << "Base NumLock pressed" << std::endl;};
        virtual void pressCapslock() {std::cout << "Base CapsLock pressed" << std::endl;};
        virtual void pressScrolllock() {std::cout << "Base ScrollLock pressed" << std::endl;};
        virtual void pressNum0() {std::cout << "Base 0 pressed" << std::endl;};
        virtual void pressNum1() {std::cout << "Base 1 pressed" << std::endl;};
        virtual void pressNum2() {std::cout << "Base 2 pressed" << std::endl;};
        virtual void pressNum3() {std::cout << "Base 3 pressed" << std::endl;};
        virtual void pressNum4() {std::cout << "Base 4 pressed" << std::endl;};
        virtual void pressNum5() {std::cout << "Base 5 pressed" << std::endl;};
        virtual void pressNum6() {std::cout << "Base 6 pressed" << std::endl;};
        virtual void pressNum7() {std::cout << "Base 7 pressed" << std::endl;};
        virtual void pressNum8() {std::cout << "Base 8 pressed" << std::endl;};
        virtual void pressNum9() {std::cout << "Base 9 pressed" << std::endl;};
        virtual void pressF1() {std::cout << "Base F1 pressed" << std::endl;};
        virtual void pressF2() {std::cout << "Base F2 pressed" << std::endl;};
        virtual void pressF3() {std::cout << "Base F3 pressed" << std::endl;};
        virtual void pressF4() {std::cout << "Base F4 pressed" << std::endl;};
        virtual void pressF5() {std::cout << "Base F5 pressed" << std::endl;};
        virtual void pressF6() {std::cout << "Base F6 pressed" << std::endl;};
        virtual void pressF7() {std::cout << "Base F7 pressed" << std::endl;};
        virtual void pressF8() {std::cout << "Base F8 pressed" << std::endl;};
        virtual void pressF9() {std::cout << "Base F9 pressed" << std::endl;};
        virtual void pressF10() {std::cout << "Base F10 pressed" << std::endl;};
        virtual void pressF11() {std::cout << "Base F11 pressed" << std::endl;};
        virtual void pressF12() {std::cout << "Base F12 pressed" << std::endl;};
    };
}