#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>


using namespace std;


string vertexShaderSource {
R"(#version 330 core
layout (location=0) in vec3 position;
layout (location=1) in vec4 color;
smooth out vec4 vColor;
void main() {
    vColor = color;
    gl_Position = vec4(position, 1.0f);
})"
};


string fragmentShaderSource {
R"(#version 330 core
out vec4 FragColor;
smooth in vec4 vColor;

void main() {
    FragColor = vColor;
})"
};


void error_callback(int error, const char* desc) {
    std::cout << desc << "\n";
};

int state;

void keyCallback(GLFWwindow* window, int key, int scanCode, int action, int mode) {
    state = glfwGetKey(window, GLFW_KEY_A);
    if(state == GLFW_PRESS) {
        cout << ".";
    }
    // switch(key) {
    //     case GLFW_KEY_ESCAPE:
    //         cout << "Window Close" << endl;
    //         glfwSetWindowShouldClose(window, true);
    //         break;
    // }
}


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

    unsigned int W = 400;
    unsigned int H = 400;
    GLFWwindow* window = glfwCreateWindow(W, H, "Triangle", nullptr, nullptr);
    if(!window) {
        cerr << "Unable to create GLFW Window" << endl;
        return -1;
    }

    glfwSetKeyCallback(window, keyCallback);

    glfwMakeContextCurrent(window);
    if(!gladLoadGL()) {
        cerr << "Glad unable to load GL" << endl;
        return -1;
    } 

    float data[] {
        0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 0.0f,   0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 0.0f,   1.0f, 0.0f, 0.0f, 1.0f,
    };

    unsigned int buffer, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &buffer);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (const void*)(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (const void*)(3 * sizeof(float)));

    char info[512];
    int buf;
    unsigned int vShader = glCreateShader(GL_VERTEX_SHADER);
    auto vStr = vertexShaderSource.c_str();
    glShaderSource(vShader, 1, &vStr, nullptr);
    glCompileShader(vShader);

    glGetShaderiv(vShader, GL_COMPILE_STATUS, &buf);
    glGetShaderInfoLog(vShader, 512, nullptr, info);
    cout << info << endl;

    unsigned int fShader = glCreateShader(GL_FRAGMENT_SHADER);
    auto fStr = fragmentShaderSource.c_str();
    glShaderSource(fShader, 1, &fStr, nullptr);
    glCompileShader(fShader);
    
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &buf);
    glGetShaderInfoLog(fShader, 512, nullptr, info);
    cout << info << endl;

    unsigned int program = glCreateProgram();
    glAttachShader(program, vShader);
    glAttachShader(program, fShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &buf);
    glGetProgramInfoLog(program, 512, nullptr, info);
    cout << info << endl;

    while (!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window);
        glfwPollEvents();

        glViewport(0, 0, W, H);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);
        glBindVertexArray(VAO);
      
        glDrawArrays(GL_TRIANGLES, 0, sizeof data / 3);
    }
    

    glfwDestroyWindow(window);
    glDeleteBuffers(1, &buffer);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(program);
    glDeleteShader(fShader);
    glDeleteShader(vShader);
    
    return 0;
}
