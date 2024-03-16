/**
 * @author github.com/bytenol
 * 
 * toggle wireframe mode
*/

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <book/shader.hpp>
#include <iostream>
#include <string>
#include <vector>


using namespace std;
using namespace mhy;


string vertexShaderSource {
R"(#version 330 core
layout (location=0) in vec3 position;
uniform mat4 VP;
smooth out float trough;

uniform float time;
const float amplitude = 0.125;
const float frequency = 4;
const float PI = 3.14159;

void main() {
    float distance = length(position);
    float y = amplitude * sin(-PI * distance * frequency * time);
    trough = abs(y);
    gl_Position = VP * vec4(position.x, y, position.z, 1.0f);
})"
};


string fragmentShaderSource {
R"(#version 330 core
out vec4 FragColor;

smooth in float trough;

void main() {
    vec3 ambient = vec3(1.0f, 1.0f, 1.0f) * 5.5f;
    vec3 color = vec3(1.0f, 1.0f, 1.0f) * trough * ambient;
    FragColor = vec4(color, 1.0f);
})"
};


inline void error_callback(int error, const char* desc) {
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

    unsigned int W = 600;
    unsigned int H = 600;
    GLFWwindow* window = glfwCreateWindow(W, H, "Ripple Mesh Deformer", nullptr, nullptr);
    if(!window) {
        cerr << "Unable to create GLFW Window" << endl;
        return -1;
    }

    glfwMakeContextCurrent(window);
    if(!gladLoadGL()) {
        cerr << "Glad unable to load GL" << endl;
        return -1;
    } 

    vector<float> vertices;

    const float amt = 3.0f;
    const float steps = 0.2f;
    for(float z{-amt}; z < amt; z += steps) {
        for(float x{-amt}; x < amt; x += steps) {
            std::vector<float> nd {
                x, 0.0f, z,
                x, 0.0f, z + steps,
                x + steps, 0.0f, z + steps,
                x, 0.0f, z,
                x + steps, 0.0f, z + steps,
                x + steps, 0.0f, z
            };
            vertices.insert(vertices.end(), nd.begin(), nd.end());
        }
    }

    unsigned int buffer, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &buffer);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (const void*)(0));

    Shader shader;
    shader.loadFromString(vertexShaderSource, fragmentShaderSource);
    shader.use();
    shader.addLocation(vector<string>{"VP", "time"});

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    auto t0 = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window);
        glfwPollEvents();

        auto t1 = glfwGetTime();
        auto dt = glfwGetTime() * 0.1f;
        t0 = t1;

        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glViewport(0, 0, W, H);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 P = glm::perspective(45.0f * 3.14159f / 180, static_cast<float>(W / H), 0.1f, 1000.0f);
        glm::mat4 V = glm::lookAt(glm::vec3{3.0f, 5.0f, -10.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});
        glm::mat4 MVP = P * V;

        glBindVertexArray(VAO);
        shader.use();
        glUniform1f(shader.getLocation("time"), dt);
        glUniformMatrix4fv(shader.getLocation("VP"), 1, false, glm::value_ptr(MVP));
        glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);
    }
    

    glfwDestroyWindow(window);
    glDeleteBuffers(1, &buffer);
    glDeleteVertexArrays(1, &VAO);
    
    return 0;
}