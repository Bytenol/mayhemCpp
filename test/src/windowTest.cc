/**
 * @file windowTest.cpp
 * @author Bytenol (github.com/bytenol)
 * @brief A simple program that display a GLFW window
 * @version 0.1
 * @date 2024-05-01
 * 
 * @copyright Copyright (c) 2024
 */

#include <mayhem/canvas.hpp>
#include <iostream>

using namespace mhy;
using namespace std;


class Application : public Canvas {

    public:

        Application(const float& w, const float& h, const string& t): Canvas(w, h, t) {
            setTitle("Window Test");
        }


    private:

        void onEnter(window_ptr window) override {
            cout << "Canvas onEnter" << endl;
            glfwSetWindowSizeCallback(window, onResize);
        }


        void onUpdate(float elapsedTime) override {

        }


        void onDraw(window_ptr window) override {
            glViewport(0, 0, static_cast<int>(getWidth()), static_cast<int>(getHeight()));
            glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
        }


        void onExit(window_ptr window) override {
            cout << "Window is about to exit" << endl;
        }


        static void onResize(window_ptr window, int w, int h) {
            cout << "Width resized from " << getWidth() << " to " << w
            << "Height resized from " << getHeight() << " to " << h << endl;
            setWidth(w);
            setHeight(h);
        }

};


int main(int argc, char const *argv[]) {
    Application window{ 400, 500, "Canvas Basic" };
    window.start();
    return 0;
}
