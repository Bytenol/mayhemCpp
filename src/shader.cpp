#include "../include/mayhem/shader.hpp"


using namespace mhy;


void Shader::loadFromString(const std::string &vertexShaderSource, const std::string &fragmentShaderSource, const std::string& geometryShaderSource) {
    vSource = vertexShaderSource;
    fSource = fragmentShaderSource;
    gSource = geometryShaderSource;
}


void Shader::loadFromFile(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& geometryShaderPath) {
    throw NotImplementedException();
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
    if(!resp) {
        const std::string inf(info);
        glDeleteShader(vShader);
        throw ShaderFailureException("VERTEX_SHADER_ERROR::" + inf);
    }
    
    unsigned int fShader = glCreateShader(GL_FRAGMENT_SHADER);
    auto fStr = fSource.c_str();
    glShaderSource(fShader, 1, &fStr, nullptr);
    glCompileShader(fShader);
    
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &resp);
    glGetShaderInfoLog(fShader, 512, nullptr, info);
    if(!resp) {
        const std::string inf(info);
        glDeleteShader(fShader);
        throw ShaderFailureException("FRAGMENT_SHADER_ERROR::" + inf);
    }

    unsigned int gShader;
    if(!gSource.empty()) {
        gShader = glCreateShader(GL_GEOMETRY_SHADER);
        auto gStr = gSource.c_str();
        glShaderSource(gShader, 1, &gStr, nullptr);
        glCompileShader(gShader);

        glGetShaderiv(gShader, GL_COMPILE_STATUS, &resp);
        glGetShaderInfoLog(gShader, 512, nullptr, info);
        if(!resp) {
            const std::string inf(info);
            glDeleteShader(gShader);
            throw ShaderFailureException("GEOMETRY_SHADER_ERROR::" + inf);
        }
    }

    program = glCreateProgram();
    glAttachShader(program, vShader);
    if(glIsShader(gShader)) glAttachShader(program, gShader);
    glAttachShader(program, fShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &resp);
    glGetProgramInfoLog(program, 512, nullptr, info);
    if(!resp) {
        const std::string inf(info);
        glDeleteProgram(program);
        throw ShaderFailureException("PROGRAM_ERROR::" + inf);
    }

    glDeleteShader(fShader);
    glDeleteShader(vShader);
    glDeleteShader(gShader);

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

    if(location[name] < 0) 
        throw ShaderFailureException("Seems like the location '" + name + "' does not exist");
}


void Shader::addLocation(const std::vector<std::string>& lst) {
    for(auto& str: lst) addLocation(str);
}


Shader::~Shader() {
    glDeleteProgram(program);
}
