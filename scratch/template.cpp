#include <mayhem/mayhem.hpp>
#include <string>


class Window: public mhy::GLRenderer {

public:
    Window(const int& w, const int& h, const std::string& t): mhy::GLRenderer(w, h, t) {}

protected:
    void onStart() override { }


    void onUpdate(float dt){ }


    void onDraw(){ }


    void onResize(){ }


    void onClose(){ }
};



int main(int argc, char const *argv[])
{
    Window w { 620, 620, "GLFW Window" };
    if(w.init()) w.start();
    else {
        std::cerr << w.getError();
        return -1;
    }
    return 0;

}
