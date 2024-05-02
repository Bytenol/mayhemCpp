/**
 * @file TransverseWave.cpp
 * @author Mustapha Ibrahim (github.com/bytenol)
 * @brief A transverse wave animation using opengl and glsl
 * @version 0.1
 * @date 2024-05-01
 * 
 * @todo Add parameters [waveSource, waveDirection, and othorgonal direction]
 * @todo implement imgui in the program
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <standalone/mayhem.hpp>


using namespace mhy;
using namespace std;


string vertexShaderSource {
    R"(#version 330 core
        layout (location=0) in vec3 position; 
        uniform mat4 projectionMatrix;
        uniform mat4 viewMatrix;

        uniform float time, amplitude, wavelength, frequency;
        smooth out float oWV;

        vec3 d = vec3(0, 0, 1); // propagation
        vec3 o = vec3(0, 0, 0);
        vec3 u = vec3(0, -1, 0); // oscillation

        const float PI = 3.14159f;

        void main() {
            float v = frequency * wavelength;
            vec3 diffPos = position - o;
            vec3 diff = diffPos * (d/v);
            float T = 1.0f / frequency * length(diffPos);
            float wv = amplitude * sin( (time - diff)/T +  PI/2.0f );
            oWV = wv/amplitude;
            vec3 disp = u * wv;
            gl_Position = projectionMatrix * viewMatrix * vec4(position + disp, 1.0f);
    })"
};


string fragmentShaderSource {
    R"(#version 330 core
        out vec4 FragColor;
        smooth in float oWV;
        void main(){
            FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f) * abs(oWV);
    })"
};



class Application : public Canvas {

    static glm::mat4 projectionMatrix;

    unsigned int vao, vbo;

    float time; 

    float amplitude = 0.5f;

    float frequency = 2.2f;

    float wavelength = 0.5f;

    glm::mat4 viewMatrix = glm::mat4(1.0f);

    vector<float> vertexData;    

    Shader shader;


    public:

        Application(const float& w, const float& h, const string& t): Canvas(w, h, t) {
            setVersion(3, 3);
        }


    private:

        static void frameBufferSizeCb(window_ptr window, int w, int h) {
            setWidth(w);
            setHeight(h);
        }


        void onEnter(window_ptr window) override {

            // initialize the vertex datas
            vertexData.clear();
            const float amt = 3.0f;
            const float steps = 0.5f;
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
                    vertexData.insert(vertexData.end(), nd.begin(), nd.end());
                }
            }

            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexData.size(), vertexData.data(), GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);

            shader.loadFromString(vertexShaderSource, fragmentShaderSource);
            shader.use();
            shader.addLocation(vector<string>{ "projectionMatrix", "viewMatrix", "amplitude", "time", "wavelength", "frequency" });
            glfwSetFramebufferSizeCallback(window, frameBufferSizeCb);
        }


        void onUpdate(float elapsedTime) override {
            time = elapsedTime;

            projectionMatrix = glm::perspective(45.0f * 3.14159f / 180, getWidth() / getHeight(), 0.1f, 1000.0f);;
            viewMatrix = glm::lookAt(glm::vec3{0.0f, 10.0f, -1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});
        }


        void onDraw(window_ptr window) override {
            glViewport(0, 0, getWidth(), getHeight());
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            shader.use();
            glUniformMatrix4fv(shader.getLocation("projectionMatrix"), 1, false, glm::value_ptr(projectionMatrix));
            glUniformMatrix4fv(shader.getLocation("viewMatrix"), 1, false, glm::value_ptr(viewMatrix));
            glUniform1f(shader.getLocation("amplitude"), amplitude);
            glUniform1f(shader.getLocation("time"), time);
            glUniform1f(shader.getLocation("wavelength"), wavelength);
            glUniform1f(shader.getLocation("frequency"), frequency);

            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, vertexData.size() / 3);
        }


        void onExit(window_ptr window) override {
            glDeleteVertexArrays(1, &vao);
            glDeleteBuffers(1, &vbo);
        }

};


glm::mat4 Application::projectionMatrix = glm::mat4(1.0f);


int main(int argc, char const *argv[])
{
    Application window{ 400, 500, "Transverse Wave" };

    window.start();

    return 0;
}
