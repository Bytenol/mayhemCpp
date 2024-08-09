#include <iostream>
#include <mayhem/mayhem.hpp>


GLFWwindow* window = nullptr;

const std::string vertexShaderSource = R"(#version 330 core
#pragma STDGL invariant(all)
#pragma optimize(on)
#pragma debug(on)

layout (location=0) in vec3 position;
layout (location=1) in vec2 texCoord;

centroid out vec2 o_texCoord;

void main() {
    o_texCoord = texCoord;
    gl_Position = vec4(position, 1.0f);
}
)";

const std::string fragmentShaderSource = R"(#version 330 core
precision highp float;
#pragma optimize(on)
#pragma debug(on)

invariant centroid in vec2 o_texCoord;

uniform sampler2D image;

out vec4 FragColor;

void main() {
    FragColor = texture(image, o_texCoord) * vec4(1, 0, 0, 1);
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

    unsigned int vao, vbo, texture;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &texture);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    float vertices[]{
        0.5f, 0.5f, 0.0f,   1.0f, 1.0f,
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,

        0.5f, 0.5f, 0.0f,   1.0f, 1.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f,  0.0f, 1.0f

    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    auto stride = 5 * sizeof(float);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, stride, (void*)(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, stride, (void*)(3 * sizeof(float)));

    auto woodenImage = mhy::loadImage((mhy::getBackDirectory(__FILE__, 2) + "/assets/images/wooden.jpg"));
    if(!woodenImage) {
        std::cout << mhy::getInfoLog() << std::endl;
        return -1;
    }
    std::cout << "Wooden image loaded successfully" << std::endl;

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, woodenImage->width, woodenImage->height, 0, GL_RGB, GL_UNSIGNED_BYTE, woodenImage->data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    stbi_image_free(woodenImage->data);


    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glfwSwapBuffers(window);
    }

    std::cout << "Exiting glfw window" << std::endl;

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &texture);
    glfwDestroyWindow(window);
    return 0;
}


