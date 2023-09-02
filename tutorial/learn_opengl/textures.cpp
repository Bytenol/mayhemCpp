#include <mayhem/mayhem.hpp>
#include <string>
#include <iostream>

using namespace std;
using namespace mhy;

const std::string vertexShaderSource {
    R"(#version 330 core
layout (location=0) in vec3 position;
layout (location=1) in vec3 color;
layout (location=2) in vec2 texCoord;
out vec3 v_color;
out vec2 v_texCoord;
void main() {
    v_color = color;
    v_texCoord = texCoord;
    gl_Position = vec4(position, 1);
}
)"
};


const std::string fragmentShaderSource {
    R"(#version 330 core
precision highp float;
out vec4 FragColor;
uniform sampler2D images[2];
in vec3 v_color;
in vec2 v_texCoord;
void main() {
    FragColor = mix(texture(images[0], v_texCoord), texture(images[1], v_texCoord), 0.25) * vec4(v_color, 1.0f);
}
)"
};



class Window: public GLRenderer {
    
    unsigned vbo, vao;
    Shader shader;
    Texture textures[2];

public:
    Window(const int& w, const int& h, const std::string& t): GLRenderer(w, h, t) {}

protected:
    void onStart() override {
        if(!shader.loadFromString(vertexShaderSource, fragmentShaderSource)) {
            setError(shader.getError());
            return;
        }

        float positions[]{
            0.0f, 0.0f, 0.0f,   0.32f, 0.54f, 0.21f,    0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,   0.65f, 0.32f, 0.75f,    1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,   0.843f, 0.543f, 0.13f,  1.0f, 1.0f,
            0.0f, 1.0f, 0.0f,    0.43f, 0.543f, 0.321f,  0.0f, 1.0f
        };

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof positions, positions, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

        glViewport(0, 0, getWidth(), getHeight());
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

        // setup initial shader values
        shader.use();

        
        textures[0] = Texture("../../../../assets/wooden.jpg", GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, true);
        if(textures[0].getError() != "") setError("Wooden texture: " + textures[0].getError());

        textures[1] = Texture("../../../../assets/f-texture.png", GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, true);
        if(textures[1].getError() != "") setError("F texture: " + textures[1].getError());

        textures[0].bind(); 
        glUniform1i(glGetUniformLocation(shader.program, "images[0]"), 0);

        textures[1].bind(); 
        glUniform1i(glGetUniformLocation(shader.program, "images[1]"), 1);

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
    Window w { 620, 620, "Textures" };
    if(w.init()) w.start();
    else {
        std::cerr << w.getError();
        return -1;
    }
    return 0;

}
