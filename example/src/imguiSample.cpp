#include <standalone/mayhem.hpp>

#include <string>

using namespace std;
using namespace mhy;

class imguiSample: public Canvas {
    
    public:
        imguiSample(const float& w, const float& h, const string& t): Canvas(w, h, t) {
            
        };

    private:
        void onEnter(window_ptr window) override;
        void onUpdate(float dt) override;
        void onDraw(window_ptr window) override;
        void onExit(window_ptr window) override;
};


int main(int argc, char const *argv[])
{
    imguiSample app{ 400, 400, "IMGUI implementation" };
    app.start();
    return 0;
}


void imguiSample::onEnter(window_ptr window) {

}


void imguiSample::onUpdate(float dt) {

}


void imguiSample::onDraw(window_ptr window) {

}


void imguiSample::onExit(window_ptr window) {

}