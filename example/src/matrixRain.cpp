/**
 * @file matrixRain.cpp
 * @author Mustapha Ibrahim (github.com/bytenol)
 * @brief Animation [Matrix rain] in 2d
 * @version 0.1
 * @date 2024-05-03
 * 
 * @todo use fileSystem to get images
 * @todo fix and resolve dt
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <standalone/mayhem.hpp>

#include <iostream>
#include <string>
#include <random>
#include <chrono>
#include <exception>
#include <filesystem>


using namespace mhy;

using namespace std;

using namespace glm;



default_random_engine random;

unsigned row, col;

float texIndexTable[22];

const float fontSize { 20.0f };     // original fontSize is 20px

const float spacing { fontSize * 0.2f };

const float texStride{ fontSize / 440.0f }; // width of the image is 440px


const std::string vertexShaderSource {
    R"(#version 330 core
layout (location=0) in vec2 position;
layout (location=1) in vec4 color;
layout (location=2) in vec2 texCoord;
uniform mat4 projectionMatrix;
out vec4 v_color;
out vec2 v_texCoord;
void main() {
    v_color = color;
    v_texCoord = texCoord;
    gl_Position = projectionMatrix * vec4(position, 0, 1);
})"
};


const std::string fragmentShaderSource {
    R"(#version 330 core
uniform sampler2D image;
in vec4 v_color;
in vec2 v_texCoord;
out vec4 FragColor;
void main() {
    FragColor = texture2D(image, v_texCoord) * v_color;
})"
};


float randRange(const float& min, const float& max) {
    std::uniform_real_distribution<float> u{ min, max };
    return u(random);
}



struct Text {

    static float speed;     // relative speed for the text animation


    /**
     * Constructor class for the matrix texts
     * @param x is the unit location of the text x=axis
     * @param y is the unit location of the text on the y=axis
     * @param texId is the unit coordinate for the texture of the text
     * @param color is the color for the texture
     * @param isStatic describe if the text is static or moves down the screen
     */
    Text(int x, int y, int texId, glm::vec4 color, bool isStatic = true) {
        this->x = x;
        this->y = y;
        this->texId = texId;
        this->color = color;
        this->isStatic = isStatic;
        textUpdateLastTime = 50.0f;
        posUpdateLastTime = 15.0f;
        oldY = y;
    }


    /**
     * This method updates the text data including their positions and alpha value
     * @param vertexData is the vector containing all vertex data to be updated as a glBufferSubdata()
     * @param backgroundTexts is the vector representing the immovable texts
     * @param col is the number of matrix text columns
     */
    //
    void updateData(float dt, std::vector<float>& vertexData, std::vector<Text>& backgroundTexts) {

        // randomly changed matrix texts
        textUpdateCounter += speed * 0.5f * dt;
        if(textUpdateCounter > textUpdateLastTime) {
            textUpdateCounter = 0.0f;
            texId = static_cast<int>(randRange(0.0f, 21.0f));
            textUpdateLastTime = !isStatic ? randRange(60.0f, 110.0f) : randRange(150.0f, 350.0f);
        }

        if(!isStatic) {
            // randomly update position
            posUpdateCounter += speed * dt;
            if(posUpdateCounter >= posUpdateLastTime) {
                posUpdateCounter = 0.0f;
                y++;
                if(y > row) {
                    y = static_cast<int>(randRange(-20, -1));
                    posUpdateLastTime = randRange(30.0f, 60.0f);
                }
                if(y >= 0) backgroundTexts[y * col + x].color.w = 0;
                if(y >= 1) backgroundTexts[(y - 1) * col + x].color.w = 255;
            }
        } else {
            color.w -= speed * 0.3f * dt;
        }

        const float sw { texIndexTable[texId] + texStride };    // width of a single texture
        const float px { static_cast<float>(x) * fontSize + spacing };
        const float py { static_cast<float>(y) * fontSize + spacing };
        const float w { px + fontSize };
        const float h { py + fontSize };

        const float alpha = !isStatic ? 1 : color.w / 255.0f;

        vertexData.insert(vertexData.end(), {
                // position[2] color[4]   text[2]
                px, py, color.x, color.y, color.z, alpha, texIndexTable[texId], 0.0f,
                w, py, color.x, color.y, color.z, alpha,  sw, 0.0f,
                w, h, color.x, color.y, color.z, alpha,   sw, 1.0f,
                px, h, color.x, color.y, color.z, alpha,  texIndexTable[texId], 1.0f
        });

    }   // updateData ends!

    // 8 is the stride of the attributes
    // 4 is the no of vertices per quad
    static int getSize(){ return sizeof(float) * 8 * 4; }

private:
    int x, y, texId, oldY;
    glm::vec4 color;
    bool isStatic;
    float textUpdateLastTime, textUpdateCounter { }, posUpdateLastTime, posUpdateCounter{ };
};

float Text::speed = 100.0f;


class Application : public Canvas {

    unsigned VBOs[3], VAO;   // position, texture, index
    Shader shader;
    vector<Text> backgroundTexts, foregroundTexts;
    vector<int> indices;
    glm::mat4 projectionMatrix;
    float timeStarted = -1.0f;

    public:

        Application(const float& w, const float& h, const string& t): Canvas(w, h, t) {
            cout << "Canvas created" << endl;
        }


    private:

        void onEnter(window_ptr window) override {
            shader.loadFromString(vertexShaderSource, fragmentShaderSource);
            shader.use();
            shader.addLocation("projectionMatrix");
            
            glGenBuffers(3, VBOs);
            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
            glBufferData(GL_ARRAY_BUFFER, (unsigned int)Text::getSize() * col * (row + 1), nullptr, GL_DYNAMIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(2 * sizeof(float)));
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOs[2]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indices.size(), indices.data(), GL_STATIC_DRAW);

            reset();

            glBindTexture(GL_TEXTURE_2D, VBOs[1]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // string imagePath = string(std::filesystem::current_path()) + "/assets/images/matrixRainText.png";

            int width, height, nrChannel;
            unsigned char *data = stbi_load("C:/Users/bytenol/Documents/Programming/Github/MayhemCPP/example/assets/images/matrixRainText.png", &width, &height, &nrChannel, 0);

            if(data) {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
            } else {
                throw std::logic_error("Unable to load matrixRainText.png");
            }

            // generate lookup table for texts
            for(int i{}; i < 22; i++) texIndexTable[i] = i * fontSize / width;

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        }

        void onUpdate(float elapsedTime) override {
            if(timeStarted < 0.0f) timeStarted = elapsedTime;
            float dt = 0.16f;//1.06 / 60.0f;
            timeStarted = elapsedTime;
            std::vector<float> vertexData;
            for(auto& m: foregroundTexts) m.updateData(dt, vertexData, backgroundTexts);
            for(auto& m: backgroundTexts) m.updateData(dt, vertexData, backgroundTexts);
            glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
            glBufferSubData(GL_ARRAY_BUFFER, 0, (int)(sizeof(float) * vertexData.size()), vertexData.data());
        }

        void onDraw(window_ptr window) override {
            glViewport(0, 0, getWidth(), getHeight());
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            glDrawElements(GL_TRIANGLES, static_cast<int>(indices.size()), GL_UNSIGNED_INT, nullptr);
        }


        void onExit(window_ptr window) override {
            glDeleteBuffers(3, VBOs);
            glDeleteVertexArrays(1, &VAO);
        }


        void reset() {
            timeStarted = -1.0f;
            indices.clear();
            backgroundTexts.clear();
            foregroundTexts.clear();
            row = (getHeight() + fontSize) / fontSize;
            col = (getWidth() + fontSize) / fontSize;

            for(int i{}; i < (row + 1) * col; i++) {
                int si{ i * 4 };
                indices.insert(indices.end(), { si, si + 1, si+ 2, si, si + 2, si + 3 });
            }

            for(int i{}; i < row; i++) {
                for(int j{}; j < col; j++) {
                    if(i == 0) foregroundTexts.push_back({ j, i, j % 21, { 1, 1, 1, 1 }, false });
                    backgroundTexts.push_back({ j, i, j % 21, { 0, 1, 0, 0 } });
                }
            }

            glBufferData(GL_ARRAY_BUFFER, (unsigned int)Text::getSize() * col * (row + 1), nullptr, GL_DYNAMIC_DRAW);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indices.size(), indices.data(), GL_STATIC_DRAW);
            glViewport(0, 0, getWidth(), getHeight());
            projectionMatrix = glm::ortho(0.0f, getWidth(), getHeight(), 0.0f);
            shader.use();
            glUniformMatrix4fv(shader.getLocation("projectionMatrix"), 1, false, glm::value_ptr(projectionMatrix));
        }


};


int main(int argc, char const *argv[])
{
    auto seedVal = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    random.seed(seedVal);

    Application window{ 400, 500, "Canvas Basic" };

    window.start();

    return 0;
}
