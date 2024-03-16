/***
 * @author github.com/bytenol
 * 
 * Refractor error report and remove IOSTREAM heade
*/
#ifndef ___BYTENOL_OPENGL_COOKBOOK__H__
#define ___BYTENOL_OPENGL_COOKBOOK__H__

#include <glad/glad.h>
#include <string>
#include <vector>
#include <map>
#include <iostream>

namespace mhy {

    class Shader {

        public:
            Shader() = default;
            void loadFromString(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
            void use();
            inline const unsigned int& getProgram() { return program; };
            void addLocation(const std::string& name);
            void addLocation(const std::vector<std::string>& lst);
            inline unsigned int getLocation(const std::string& name) const;
            ~Shader();

        private:
            bool isInitialized = false;
            std::string vSource, fSource;
            unsigned int program;
            std::map<std::string, unsigned int> location;
            void initProgram();
    };



    void Shader::loadFromString(const std::string &vertexShaderSource, const std::string &fragmentShaderSource)
    {
        vSource = vertexShaderSource;
        fSource = fragmentShaderSource;
    }


    void Shader::initProgram() {
        char info[512];
        int resp;
        unsigned int vShader = glCreateShader(GL_VERTEX_SHADER);
        auto vStr = vSource.c_str();
        glShaderSource(vShader, 1, &vStr, nullptr);
        glCompileShader(vShader);

        glGetShaderiv(vShader, GL_COMPILE_STATUS, &resp);
        glGetShaderInfoLog(vShader, 512, nullptr, info);
        if(!resp) std::cout << "VERTEX ERROR::" << info << std::endl;
        
        unsigned int fShader = glCreateShader(GL_FRAGMENT_SHADER);
        auto fStr = fSource.c_str();
        glShaderSource(fShader, 1, &fStr, nullptr);
        glCompileShader(fShader);
        
        glGetShaderiv(fShader, GL_COMPILE_STATUS, &resp);
        glGetShaderInfoLog(fShader, 512, nullptr, info);
        if(!resp) std::cout << "FRAGMENT ERROR::" << info << std::endl;

        program = glCreateProgram();
        glAttachShader(program, vShader);
        glAttachShader(program, fShader);
        glLinkProgram(program);

        glGetProgramiv(program, GL_LINK_STATUS, &resp);
        glGetProgramInfoLog(program, 512, nullptr, info);
        if(!resp) std::cout << "PROGRAM ERROR::" << info << std::endl;
    }


    void Shader::use() {
        if(!isInitialized) {
            initProgram();
            glUseProgram(program);
            isInitialized = true;
            return;
        }
        glUseProgram(program);
    }


    void Shader::addLocation(const std::string& name) {
        const char* cpy = name.c_str();
        location[name] = glGetUniformLocation(program, cpy);
    }


    void Shader::addLocation(const std::vector<std::string>& lst) {
        for(auto& str: lst) addLocation(str);
    }


    inline unsigned int Shader::getLocation(const std::string& name) const {
        return location.at(name);
    }


    Shader::~Shader() {
        glDeleteProgram(program);
    }

}

#endif 