/**
 * @file longitudinalWave.cpp
 * @author Mustapha Ibrahim (github.com/bytenol)
 * @brief A longitudinal wave animation using opengl and glsl
 * @version 0.1
 * @date 2024-05-01
 *
 * @copyright Copyright (c) 2024
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

        uniform float time;
        uniform mat4 projectionMatrix, viewMatrix;
        uniform vec3 waveSource, waveParams, waveDir;

        out float oWV;

        void main() {
            float x = length(waveSource - position);
            float w = 2.0f * 3.14159f * waveParams[1];
            float c = waveParams[2] * waveParams[1];
            float wv = waveParams[0] * cos(w * (time - x/c));
            oWV = wv;
            vec3 disp = position + (waveDir * wv);
            gl_Position = projectionMatrix * viewMatrix * vec4(disp, 1.0f);
        })"
};


string fragmentShaderSource {
    R"(#version 330 core
        out vec4 FragColor;
        in float oWV;

        const float intensity = 2.4f;

        void main(){
            vec4 ambient = vec4(1.0f, 1.0f, 1.0f, 1.0f) * intensity;
            FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f) * abs(oWV) * ambient;
        })"
};



class Application : public Canvas {

    static glm::mat4 projectionMatrix;

    unsigned int vao, vbo;

    float time; 

    float amplitude = 0.5f;

    float frequency = 0.2f;

    float wavelength = 1.0f;

    glm::mat4 viewMatrix = glm::mat4(1.0f);

    glm::vec3 waveParams;

    glm::vec3 waveSource = { 0.0f, 0.0f, 0.0f };

    glm::vec3 waveDir = {0.1f, 0.2f, -0.2f };

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
            shader.addLocation(vector<string>{ "projectionMatrix", "viewMatrix", "waveSource", "waveParams", "waveDir", "time"});
            glfwSetFramebufferSizeCallback(window, frameBufferSizeCb);
        }


        void onUpdate(float elapsedTime) override {
            time = elapsedTime;

            projectionMatrix = glm::perspective(45.0f * 3.14159f / 180, getWidth() / getHeight(), 0.1f, 500.0f);
            viewMatrix = glm::lookAt(glm::vec3{0.0f, 10.0f, -1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});
            waveParams.x = amplitude;
            waveParams.y = frequency;
            waveParams.z = wavelength;
        }


        void onDraw(window_ptr window) override {
            glViewport(0, 0, getWidth(), getHeight());
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            shader.use();
            glUniform1f(shader.getLocation("time"), time);
            glUniform3fv(shader.getLocation("waveSource"), 1, glm::value_ptr(waveSource));
            glUniform3fv(shader.getLocation("waveDir"), 1, glm::value_ptr(waveDir));
            glUniform3fv(shader.getLocation("waveParams"), 1, glm::value_ptr(waveParams));
            glUniformMatrix4fv(shader.getLocation("projectionMatrix"), 1, false, glm::value_ptr(projectionMatrix));
            glUniformMatrix4fv(shader.getLocation("viewMatrix"), 1, false, glm::value_ptr(viewMatrix));

            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, vertexData.size() / 3);
        }


        void onExit(window_ptr window) override {
            glDeleteVertexArrays(1, &vao);
            glDeleteBuffers(1, &vbo);
        }

};


glm::mat4 Application::projectionMatrix = glm::mat4(1.0f);


int main(int argc, char const *argv[]) {
    Application window{ 400, 500, "Longitudinal Wave" };
    window.start();

    return 0;
}
