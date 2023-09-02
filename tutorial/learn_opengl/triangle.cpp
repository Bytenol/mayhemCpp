#include <mayhem/mayhem.hpp>
#include <string>
#include <iostream>


using namespace std;
using namespace mhy;

const std::string vertexShaderSource {
    R"(#version 330 core
layout (location=0) in vec3 position;
uniform mat4 projectionMatrix;
void main() {
    gl_Position = projectionMatrix * vec4(position, 1);
}
)"
};


const std::string fragmentShaderSource {
    R"(#version 330 core
out vec4 FragColor;
void main() {
    FragColor = vec4(1.0f, 0.32f, 0.65f, 1.0f);
}
)"
};



class Window: public GLRenderer {
    
    unsigned vbo, vao;
    Shader shader;

public:
    Window(const int& w, const int& h, const std::string& t): GLRenderer(w, h, t) {}

protected:
    void onStart() override {
        if(!shader.loadFromString(vertexShaderSource, fragmentShaderSource)) {
            setError(shader.getError());
            return;
        }

        const float sz { 100.0f };
        float positions[]{
            0.0f, 0.0f,
            sz, 0.0f, 
            sz, sz,
            0.0f, sz
        };

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof positions, positions, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

        glViewport(0, 0, getWidth(), getHeight());
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

        // setup initial shader values
        shader.use();
        glm::mat4 projectionMatrix { glm::ortho(0.0f, static_cast<float>(getWidth()), static_cast<float>(getHeight()), 0.0f) };
        shader.setUniform("projectionMatrix", glm::value_ptr(projectionMatrix), 1);
    }

    void onUpdate(float dt) override {

    }

    void onDraw() override {
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }

    void onClose() override {
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &vao);
    }
};



int main(int argc, char const *argv[])
{
    Window w { 620, 620, "Triangle" };
    if(w.init()) w.start();
    else {
        std::cerr << w.getError();
        return -1;
    }
    return 0;

}
