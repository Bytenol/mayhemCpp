#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <random>
#include <glm/glm/glm.hpp>
#include <glm/glm/ext.hpp>
#include <glm/glm/common.hpp>
#include <mayhem/mayhem.hpp>


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

const unsigned int W = 400;
const unsigned int H = 500;


class Point {

    float ax, ay;
    float mass = 1.0f;

    public:
        float x, y, vx, vy;
        glm::mat4 mModel;
        Point() = default;
        Point(float _x, float _y) {
            x = _x;
            y = _y;
            ox = _x;
            oy = _y;
        };

        void update(float dt) {
            vx = x - ox;
            vy = y - oy;

            ax = (0.0f) / mass;
            ay = (70.0f) / mass;

            vx += ax * dt;
            vy += ay * dt;

            ox = x;
            oy = y;

            if(y >= H) {
                ox = x;
                oy = y;
                x = ox;
                // y = H - vy * 0;
            }
            x += vx * dt;
            y += vy * dt;
        }

        glm::mat4 getModel() {
            return glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f)) *
                glm::scale(glm::mat4{1.0f}, glm::vec3{10.0f, 10.0f, 0.0f});
        }

    private:
        float ox, oy;
};


class Stick {
    public:
        float length;
        int p1, p2;
        Stick(int _p1, int _p2, float l) {
            p1 = _p1;
            p2 = _p2;
            length = l;
        }

    void update(std::vector<Point>& points, float dt) {
        auto& point1 = points[p1];
        auto& point2 = points[p2];

        auto dx = glm::abs(point1.x - point2.x);
        auto dy = glm::abs(point1.y - point2.y);
        auto l = glm::sqrt(dx * dx + dy * dy);
        auto diff = glm::abs(length - l) * 0.5f;
        dx /= l;
        dy /= l;
        point1.x += dx * diff;
        point1.y += dy * diff;
        point1.x -= dx * diff;
        point1.y -= dy * diff;
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

    auto lVbo = mhy::createBuffer(mhy::BufferType::VERTEX_BUFFER_OBJECT);
    // auto lIbo = mhy::createBuffer(mhy::BufferType::ELEMENT_ARRAY_OBJECT);
    glBufferData(GL_ARRAY_BUFFER, 1000, nullptr, GL_STREAM_DRAW);

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
    points.push_back({ 30.0f, 30.0f });
    // points.push_back({ 30.0f, 100.0f });
    std::vector<Stick> sticks;
    // sticks.push_back({ 0, 1, 100.0f });

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
        }

        for(auto& stick: sticks) {
            stick.update(points, dt);
        }

        std::vector<float> lines;
        mhy::bindBuffer(vao);
        mhy::bindBuffer(vbo);
        for(auto& point: points) {
            lines.push_back(point.x);
            lines.push_back(point.y);
            glUniformMatrix4fv(shader.getLocation("mModel"), 1, false, glm::value_ptr(point.getModel()));
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(0));
        }

        mhy::bindBuffer(lVbo);
        auto res = glm::mat4(1.0f);
        glUniformMatrix4fv(shader.getLocation("mModel"), 1, false, glm::value_ptr(res));
        for(auto& stick: sticks) {  
            // point.update(dt);
            // glUniformMatrix4fv(shader.getLocation("mModel"), 1, false, glm::value_ptr(point.mModel));
            // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(0));
        }
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * lines.size(), lines.data(), GL_STREAM_DRAW);
        glDrawArrays(GL_LINES, 0, lines.size());



        glfwSwapBuffers(window);
    }

    std::cout << "Exiting glfw window" << std::endl;

    glfwDestroyWindow(window);
    return 0;
}