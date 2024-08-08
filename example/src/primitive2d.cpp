#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>
#include <mayhem/mayhem.hpp>


GLFWwindow* window = nullptr;

const std::string vertexShaderSource = R"(#version 330 core
#pragma STDGL invariant(all)
#pragma optimize(on)
#pragma debug(on)

layout (location=0) in vec3 position;

void main() {
    gl_Position = vec4(position, 1.0f);
}
)";

const std::string fragmentShaderSource = R"(#version 330 core
precision highp float;

out vec4 FragColor;

void main() {
    FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
)";



int main(int argc, char const *argv[])
{
    if(!glfwInit()) {
        std::cerr << "Failed to initalize glfw" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(400, 500, "primitive", nullptr, nullptr);
    if(!window) {
        std::cerr << "Unable to create GLFW window" << std::endl;
        return -1;
    }

    glfwMakeContextCurrent(window);
    if(!gladLoadGL()) {
        std::cout << "GLAD unable to load opengl" << std::endl;
        return -1;
    }

    std::cout << "Window created successfully" << std::endl;

    mhy::Shader shader;
    shader.loadFromString(vertexShaderSource, fragmentShaderSource);
    shader.use();

    unsigned int vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    float vertices[]{
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, (void*)(0));

    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);
    }

    std::cout << "Exiting glfw window" << std::endl;

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glfwDestroyWindow(window);
    return 0;
}


