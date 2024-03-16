#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

void error_callback(int error, const char* desc) {
    std::cout << desc << "\n";
};


int main(int argc, char const *argv[])
{
    if(!glfwInit()) {
        std::cerr << "GLFW initialization failed" << endl;
        return -1;
    }

    glfwSetErrorCallback(error_callback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(400, 400, "My window", nullptr, nullptr);
    if(!window) {
        cerr << "Unable to create GLFW Window" << endl;
        return -1;
    }

    glfwMakeContextCurrent(window);
    if(!gladLoadGL()) {
        cerr << "Glad unable to load GL" << endl;
        return -1;
    } 

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
    }
    

    glfwDestroyWindow(window);
    
    return 0;
}
