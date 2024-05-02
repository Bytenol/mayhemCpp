/**
 * @file canvas.cpp
 * @author Mustapha Ibrahim (github.com/bytenol)
 * @brief A simple program that display a GLFW window
 * @version 0.1
 * @date 2024-05-01
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <standalone/mayhem.hpp>

using namespace mhy;
using namespace std;


class Application : public Canvas {

    public:

        Application(const float& w, const float& h, const string& t): Canvas(w, h, t) {
            cout << "Canvas created" << endl;
        }


    private:

        void onEnter(window_ptr window) override {
            cout << "Canvas onEnter" << endl;
        }

        void onUpdate(float elapsedTime) override {

        }

        void onDraw(window_ptr window) override {
            glViewport(0, 0, getWidth(), getHeight());
            glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
        }

};


int main(int argc, char const *argv[])
{
    Application window{ 400, 500, "Canvas Basic" };

    window.start();

    return 0;
}
