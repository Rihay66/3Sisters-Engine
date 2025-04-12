#pragma  once

#ifndef TEST_WINDOW_HPP
#define TEST_WINDOW_HPP

#include <cameras/ortho_camera.hpp>
#include <window/sisters_glfw_window.hpp>
#include "input/sisters_glfw_gamepad.hpp"

using namespace GLFW;

class TestWindow : public Window {
    private:
        OrthoCamera camera;
        // private storage of a gamepad
        Gamepad pad;
    public:
        TestWindow();
        ~TestWindow();

        void init() override;
        void update() override;
        void stepUpdate(double ts) override;

        void render(double alpha) override;
};

#endif
