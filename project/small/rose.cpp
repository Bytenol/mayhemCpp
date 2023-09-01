#include <mayhem/mayhem.hpp>
#include <string>

const std::string vertexShaderSource {
    R"(#version 330 core
layout (location=0) in vec3 a_position;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
out vec4 position;
void main() {
    position = modelMatrix * vec4(a_position, 1);
    gl_Position = projectionMatrix * position;
})"
};


const std::string fragmentShaderSource {
    R"(#version 330 core
#define PI 3.14159

out vec4 FragColor;
uniform float time;
uniform vec2 resolution;
in vec4 position;

const float radius = 100.0f;

void main() {
    FragColor = vec4(vec2(position.x, position.y) / resolution, 1.0f / sin(time * 0.03f), 1.0f);
    FragColor.rgb *= sin(time * 0.2f) * cos(normalize(position).x);
})"
};



class Window: public mhy::GLRenderer {
    
    unsigned vao, vbo;
    mhy::Shader shader;
    glm::mat4 projectionMatrix, modelMatrix;
    glm::vec2 resolution;

public:
    Window(const int& w, const int& h, const std::string& t): mhy::GLRenderer(w, h, t) {}

protected:
    void onStart() override { 

        if(!shader.loadFromString(vertexShaderSource, fragmentShaderSource)) {
            setError(shader.getError());
            return;
        }

        shader.use();

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        float positions[]{
            0.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
        };

        glBufferData(GL_ARRAY_BUFFER, sizeof positions, positions, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        onResize();
    }


    void onUpdate(float dt){ 
        float time[]{ getElapsedTime() };
        shader.setUniform("time", time);
    }


    void onDraw(){ 
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }


    void onResize(){
        glViewport(0, 0, getWidth(), getHeight());
        resolution = glm::vec2(static_cast<float>(getWidth()), static_cast<float>(getHeight()));
        shader.setUniform("resolution", glm::value_ptr(resolution));
        projectionMatrix = glm::ortho(0.0f, resolution.x, resolution.y, 0.0f);
        shader.setUniform("projectionMatrix", glm::value_ptr(projectionMatrix));
        modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(resolution, 0.0f));
        shader.setUniform("modelMatrix", glm::value_ptr(modelMatrix));
    }


    void onClose(){
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }
};



int main(int argc, char const *argv[])
{
    Window w { 620, 620, "Rose Animation" };
    if(w.init()) w.start();
    else {
        std::cerr << w.getError();
        return -1;
    }
    return 0;

}
