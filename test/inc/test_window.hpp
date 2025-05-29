#pragma  once

#ifndef TEST_WINDOW_HPP
#define TEST_WINDOW_HPP

#include <window/sisters_sdl_window.hpp>
#include <input/sisters_sdl_gamepad.hpp>
#include <cameras/ortho_camera.hpp>

using namespace SDL;

class TestWindow : public Window {
    private:
        OrthoCamera camera;
        // private storage of a gamepad
        Gamepad pad;
    public:

        void init() override;
        void update() override;
        void stepUpdate(double ts) override;

        void render(double alpha) override;
};

#endif
