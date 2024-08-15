#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <random>
#include <glm/glm/glm.hpp>
#include <glm/glm/ext.hpp>
#include <glm/glm/common.hpp>
#include <mayhem/mayhem.hpp>
#include "./stoodVerly.h"


GLFWwindow* window = nullptr;

const std::string vertexShaderSource = R"(#version 330 core
#pragma STDGL invariant(all)
#pragma optimize(on)
#pragma debug(on)

layout (location=0) in vec2 position;
uniform mat4 mProjection;
uniform mat4 mModel;

void main() {
    gl_Position = mProjection * mModel * vec4(position, 0.0f, 1.0f);
}
)";

const std::string fragmentShaderSource = R"(#version 330 core
precision highp float;
#pragma optimize(on)
#pragma debug(on)


out vec4 FragColor;

void main() {
    FragColor = vec4(1, 0, 0, 1);
}
)";

const unsigned int W = 500;
const unsigned int H = 400;



class Point {

    float mass = 1.0f;
    vly::Vector gravity { 0.0f, 1.0f };
    float friction = 0.97f;
    float groundFriction = 0.97f;

    vly::Vector acc;
    vly::Vector vel;

    public:
        vly::Vector pos;
        vly::Vector oldPos;

        Point() = default;

        Point(const vly::Vector& _pos) {
            oldPos = _pos;
            pos = _pos;
        };

        void update(float dt) {
            vel = pos - oldPos;

            oldPos = pos;
            // vel += calc_force();
            pos += vel;
        }

        void constrain() {
            if(pos.y >= H) {
                pos.y = H;
            }
            if(pos.x <= 0) {
                pos.x = 0;
            }
            if(pos.x >= W) {
                pos.x = W;
            }
        }

        glm::mat4 getModel() {
            return glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, 0.0f)) *
                glm::scale(glm::mat4{1.0f}, glm::vec3{10.0f, 10.0f, 0.0f});
        }

        vly::Vector calc_acc() {
            return (gravity) / mass;
        }

    private:
        float ox, oy;
};


class Stick {
    public:
        float length, stiffness = 2.0f;
        Point* p1 = nullptr;
        Point* p2 = nullptr;

        Stick(std::vector<Point>& points, int _p1, int _p2, float l) {
            p1 = &(points[_p1]);
            p2 = &(points[_p2]);
            length = l;
        }

    void update(float dt) {
        auto dPos = p2->pos - p1->pos;
        auto norm = dPos.normalize();
        auto dist = dPos.length();
        auto diff = (length - dist) / 2;
        p1->pos += (norm * diff);
        p2->pos -= (norm * diff);
    }
};

int main(int argc, char const *argv[])
{
    if(!glfwInit()) {
        std::cerr << "Failed to initalize glfw" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(W, H, "verlet physics 1", nullptr, nullptr);
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
    shader.addLocation(std::vector<std::string>{ "mProjection", "mModel" });

    auto mProj = glm::ortho(0.0f, static_cast<float>(W), static_cast<float>(H), 0.0f);
    glUniformMatrix4fv(shader.getLocation("mProjection"), 1, false, glm::value_ptr(mProj));

    auto lvao = mhy::createBuffer(mhy::BufferType::VERTEX_ARRAY_OBJECT);
    mhy::bindBuffer(lvao);
    auto lvbo = mhy::createBuffer(mhy::BufferType::VERTEX_BUFFER_OBJECT);
    float lvertices[]{
        0.0f, 0.0f,
        200.0f, 200.0f
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(lvertices), lvertices, GL_STREAM_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, (void*)(0));


    auto vao = mhy::createBuffer(mhy::BufferType::VERTEX_ARRAY_OBJECT);
    mhy::bindBuffer(vao);
    
    auto vbo = mhy::createBuffer(mhy::BufferType::VERTEX_BUFFER_OBJECT);
    auto ibo = mhy::createBuffer(mhy::BufferType::ELEMENT_ARRAY_OBJECT);

    float vertices[]{
        0.5f, 0.5f,  
        0.5f, -0.5f, 
        -0.5f, -0.5f,
        -0.5f, 0.5f, 
    };

    int indices[] { 0, 1, 2, 0, 2, 3 };

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof indices, indices, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    auto stride = 2 * sizeof(float);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, stride, (void*)(0));

    std::vector<Point> points;
    points.push_back({ vly::Vector{ 30.0f, 30.0f } });
    points.push_back({ vly::Vector{130.0f, 100.0f} });
    std::vector<Stick> sticks;
    sticks.push_back( Stick{ points, 0, 1, 100.0f });

    auto t1 = std::chrono::high_resolution_clock::now().time_since_epoch();

    while(!glfwWindowShouldClose(window)) {
        auto t2 = std::chrono::high_resolution_clock::now().time_since_epoch();
        auto dt = (t2 - t1).count() * 10e-9f;
        t1 = t2;
        glfwPollEvents();
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        for(auto& point: points) {  
            point.update(dt);
            point.constrain();
        }

        for(auto& stick: sticks) {
            // stick.update(dt);
        }

        std::vector<float> lines;
        mhy::bindBuffer(vao);
        for(auto& point: points) {
            lines.push_back(point.pos.x);
            lines.push_back(point.pos.y);
            glUniformMatrix4fv(shader.getLocation("mModel"), 1, false, glm::value_ptr(point.getModel()));
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(0));
        }

        
        for(auto& stick: sticks) {  
            // point.update(dt);
            // glUniformMatrix4fv(shader.getLocation("mModel"), 1, false, glm::value_ptr(point.mModel));
            // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(0));
        }



        glfwSwapBuffers(window);
    }

    std::cout << "Exiting glfw window" << std::endl;

    glfwDestroyWindow(window);
    return 0;
}