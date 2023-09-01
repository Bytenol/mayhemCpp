/**
 * @Author: Mustapha Ibrahim A.
 * @Created: 16th August, 2023
 * @LastModified: 16th August, 2023
 *
 * This is a sample demo of the matrix rain animation. This demo utilises random japanese
 * texts to depict the encrypted data. The texts changes randomly
 *
 * @todo
 * fix path for images
 */
#include <mayhem/mayhem.hpp>
#include <string>
#include <iostream>
#include <random>
#include <ctime>

#define STB_IMAGE_IMPLEMENTATION
#include <misc/stb_image.h>

class Text;

std::default_random_engine random;
int mapSize;
float texIndexTable[22];
const float fontSize { 20.0f };
const float spacing { fontSize * 2.5f };
const float texStride{ fontSize / 440.0f }; // width of the image is 440px
const int worldHeight { 200 };
const float PI { 3.1459f };
std::vector<std::vector<std::vector<Text>>> backgroundTexts;
std::vector<Text> foregroundTexts;

const std::string vertexShaderSource {
        R"(#version 330 core
layout (location=0) in vec3 position;
layout (location=1) in vec4 color;
layout (location=2) in vec2 texCoord;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
out vec4 v_color;
out vec2 v_texCoord;
void main() {
    v_color = color;
    v_texCoord = texCoord;
    gl_Position = projectionMatrix * viewMatrix * vec4(position, 1);
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

struct vec4 { float x, y, z, w; };


struct Text {
    /**
     * Constructor class for the matrix texts
     * @param x is the unit location of the text x=axis
     * @param y is the unit location of the text on the y=axis
     * @param texId is the unit coordinate for the texture of the text
     * @param color is the color for the texture
     * @param isStatic describe if the text is static or moves down the screen
     */
    Text(int x, int y, int z, bool isStatic = true) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->isStatic = isStatic;
        this->color = isStatic ? vec4{ 0, 1, 0, 1 }: vec4{ 1, 1, 1, 1};
        textUpdateLastTime = 100.0f;
        posUpdateLastTime = 30.0f;
        oldY = y;
    }

    /**
     * This method updates the text data including their positions and alpha value
     * @param vertexData is the vector containing all vertex data to be updated as a glBufferSubdata()
     * @param backgroundTexts is the vector representing the immovable texts
     * @param col is the number of matrix text columns
     */
    void updateData(std::vector<float>& vertexData) {

        // randomly changed matrix texts
        textUpdateCounter += 1.0f;
        if(textUpdateCounter > textUpdateLastTime) {
            textUpdateCounter = 0.0f;
            texId = static_cast<int>(randRange(0.0f, 21.0f));
            textUpdateLastTime = !isStatic ? randRange(60.0f, 110.0f) : randRange(150.0f, 350.0f);
        }

        if(!isStatic) {
            // randomly update position
            posUpdateCounter += 1.0f;
            if(posUpdateCounter >= posUpdateLastTime) {
                posUpdateCounter = 0.0f;
                y++;
                if(y >= mapSize) {
                    y = 0;
                    posUpdateLastTime = randRange(30.0f, 60.0f);
                }
                if(y >= 0) backgroundTexts[z][y][x].color.w = 0;
                if(y >= 1) backgroundTexts[z][y - 1][x].color.w = 255;
            }
        } else {
            color.w -= 0.25f;
        }

        const float sw { texIndexTable[texId] + texStride };    // width of a single texture
        const float px { static_cast<float>(x) * spacing };
        const float py { static_cast<float>(y) * spacing };
        const float pz { static_cast<float>(z) * -spacing };
        const float w { px + fontSize };
        const float h { py + fontSize };

        const float alpha = !isStatic ? 1 : color.w / 255.0f;

        vertexData.insert(vertexData.end(), {
                // position[2] color[4]   text[2]
                px, py, pz, color.x, color.y, color.z, alpha, texIndexTable[texId], 0.0f,
                w, py, pz, color.x, color.y, color.z, alpha,  sw, 0.0f,
                w, h, pz, color.x, color.y, color.z, alpha,   sw, 1.0f,
                px, h, pz, color.x, color.y, color.z, alpha,  texIndexTable[texId], 1.0f
        });

    }   // updateData ends!

    // 9 is the stride of the attributes
    // 4 is the no of vertices per quad
    static int getSize(){ return sizeof(float) * 9 * 4; }

private:
    int x, y, z, texId{ }, oldY;
    vec4 color{};
    bool isStatic;
    float textUpdateLastTime, textUpdateCounter { }, posUpdateLastTime, posUpdateCounter{ };
};


class MatrixRain: public mhy::GLRenderer {

    int matrixGridSize;

    unsigned VBOs[3], VAO;   // position, texture, index
    mhy::Shader shader;
    std::vector<int> indices;

    glm::vec3 camPos;
    float camPosInc{ };
    glm::vec3 camRot;

public:
    MatrixRain(): mhy::GLRenderer(620, 620, "Matrix Rain 3D"){}

protected:
    void onStart() override {
        if(!shader.loadFromString(vertexShaderSource, fragmentShaderSource)) {
            std::cout << shader.getError();
            return;
        }

        random.seed(time(nullptr));

        // set matrix grid square size from the highest dimension
        mapSize = 10; //static_cast<int>((getHeight() > getWidth() ? getHeight() : getWidth()) / fontSize);

        // setup index buffer data
        matrixGridSize = mapSize * mapSize * mapSize + mapSize * mapSize;
        for(int i{}; i < matrixGridSize; i++) {
            int si{ i * 4 };
            indices.insert(indices.end(), { si, si + 1, si+ 2, si, si + 2, si + 3 });
        }

        for(int z{}; z < mapSize; z++) {
            backgroundTexts.push_back({});
            for(int y{}; y < mapSize; y++) {
                backgroundTexts[z].push_back({});
                for(int x{}; x < mapSize; x++) {
                    if(z == 0) {
                        foregroundTexts.push_back({ x, 0, y, false });
                    }
                    backgroundTexts[z][y].push_back({x, y, z });
                }
            }
        }

        glGenBuffers(3, VBOs);
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
        glBufferData(GL_ARRAY_BUFFER, Text::getSize() * matrixGridSize, nullptr, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOs[2]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indices.size(), indices.data(), GL_STATIC_DRAW);

        glBindTexture(GL_TEXTURE_2D, VBOs[1]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int width, height, nrChannel;
        unsigned char *data = stbi_load("../../assets/matrixRain.png", &width, &height, &nrChannel, 0);

        if(data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::cout << "Unable to load image the provided image" << std::endl;
            return;
        }

        for(int i{}; i < 22; i++) texIndexTable[i] = i * fontSize / width;

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH);
        shader.use();
    }

    void onUpdate(float dt) override {
        std::vector<float> vertexData;
        for(auto& m: foregroundTexts) m.updateData(vertexData);
        for(int i{}; i < mapSize; i++) {
            for(int j{}; j < mapSize; j++) {
                for(int k{}; k < mapSize; k++) {
                    backgroundTexts[i][j][k].updateData(vertexData);
                }
            };
        }
        glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, (int)(sizeof(float) * vertexData.size()), vertexData.data());

        camPosInc += 0.0023f;
        float rng = (mapSize / 3) * spacing;
        camPos = { 0.0f, 0.0f, -100.0f };
//        camPos.x = -100.0f + std::sin(camPosInc * 0.32f) * rng;
//        camPos.y = -100.0f + std::sin(camPosInc * 0.41f) * rng;
//        camPos.z = -100.0f + std::sin(camPosInc) * rng;


//        camRot.x = camPosInc;
//        camRot.y = camPosInc * 0.32f;
//        camRot.z = camPosInc * 0.41f;
//        camRot = { 0.0f, 0.0f, PI };
    }

    void onDraw() override {
        glViewport(0, 0, getWidth(), getHeight());
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        const float aspect = getWidth() / getHeight();
        glm::mat4 projectionMatrix { glm::perspective(45.0f * 3.14159f / 180.0f, aspect, 0.1f, 1000.f) };
        glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -300.0f));
        shader.setUniform("projectionMatrix", glm::value_ptr(projectionMatrix));
        shader.setUniform("viewMatrix", glm::value_ptr(viewMatrix));

        glDrawElements(GL_TRIANGLES, static_cast<int>(indices.size()), GL_UNSIGNED_INT, nullptr);
    }

    void onClose() override {
        glDeleteBuffers(3, VBOs);
        glDeleteVertexArrays(1, &VAO);
    }

};

int main() {
    MatrixRain game;
    if(game.init()) game.start();
    else {
        std::cout << game.getError();
        return -1;
    }
    return 0;
}//
// Created by bytenol on 8/24/2023.
//
