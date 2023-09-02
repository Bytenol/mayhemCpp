#include <mayhem/mayhem.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <misc/stb_image.h>


namespace mhy {


    /// @brief Constructor for the GLFW based window
    /// @param width is the initial width of the window
    /// @param height is the initial height of the window
    /// @param title is the title of the window
    GLRenderer::GLRenderer(int width, int height, std::string title) {
        size[0] = width;
        size[1] = height;
        info[0] = title;
    }


    /// @brief  initialises glfw and window's parameters
    /// @return boolean indicating wether the initalisation was successful
    bool GLRenderer::init() {

        // initialise glfw
        if(!glfwInit()) {
            setError("Unable to initialise GLFW");
            return false;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, getVersionMinor());
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, getVersionMajor());
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(300, 150, "", nullptr, nullptr);

        if(!window) {
            setError("GLFW unable to create window for opengl " + std::to_string(getVersionMajor()) + "." + std::to_string(getVersionMinor()));
            return false;
        }

        setTitle(info[0]);
        setSize(size[0], size[1]);

        glfwMakeContextCurrent(window);

        // load opengl
        if(!gladLoadGL()) {
            setError("Glad unable to load opengl profile");
            return false;
        }

        setError("");

        auto seedVal = std::chrono::high_resolution_clock::now();
        random.seed(seedVal.time_since_epoch().count());

        onStart();
        return getError() == "";
    }


    const std::string& GLRenderer::getError() const {
        return info[1];
    }


    void GLRenderer::start() {
        lastTime = std::chrono::high_resolution_clock::now();
        elapsedTimeStarted = std::chrono::high_resolution_clock::now();
        elapsedTime = 0.0f;

        // here is the main loop for the renderer
        // it has precalculated the delta and elapsed time
        // it also poll events and check if window size has changed
        // it calls the onUpdate and onDraw function
        // it writes window data such as title and time info
        while (!glfwWindowShouldClose(window)) {

            // calculating time, elapsed and delta
            auto now = std::chrono::high_resolution_clock::now();
            auto dt = (now - lastTime).count() * 10e-9;
            if(dt >= 0.2) dt = 0;
            lastTime = now;
            elapsedTime = (now - elapsedTimeStarted).count() * 10e-9;

            glfwPollEvents();

            // resizing the window event
            glfwGetWindowSize(window, currentSize, currentSize + 1);
            if(currentSize[0] != size[0] || currentSize[1] != size[1]) {
                setSize(currentSize[0], currentSize[1]);
                onResize();
            }

            onUpdate(dt);
            onDraw(); 

            std::string t { getTitle() + "  --- dt: " + std::to_string(dt) + " --- et: " + std::to_string(elapsedTime) };
            glfwSetWindowTitle(window, t.c_str());
            glfwSwapBuffers(window);
        }
        
    }



    void GLRenderer::setVersion(const int& major, const int& minor) {
        version[0] = major;
        version[1] = minor;
    }


    void GLRenderer::setSize(const int& width, const int& height) {
        size[0] = width;
        size[1] = height;
        glfwSetWindowSize(window, width, height);
    }


    void GLRenderer::setTitle(const std::string& t) {
        info[0] = t;
    }


    void GLRenderer::setError(const std::string& t) {
        info[1] = t;
    }


    GLFWwindow* GLRenderer::getContext() const {
        return window;
    }


    const int& GLRenderer::getVersionMajor() const { 
        return version[0]; 
    }


    const int& GLRenderer::getVersionMinor() const { 
        return version[1]; 
    }


    const int& GLRenderer::getWidth() const { 
        return size[0]; 
    }


    const int& GLRenderer::getHeight() const { 
        return size[1]; 
    }


    const std::string& GLRenderer::getTitle() const { 
        return info[0]; 
    }


    const float& GLRenderer::getElapsedTime() const {
        return elapsedTime;
    }


    GLRenderer::~GLRenderer() {
        onClose();
        glfwDestroyWindow(window);
        glfwTerminate();
    }


    bool Shader::loadFromString(const std::string &vertexShaderSource, const std::string &fragmentShaderSource) {
        bool isVertexShader = initShader(GL_VERTEX_SHADER, vertexShader, vertexShaderSource);
        bool isFragmentShader = initShader(GL_FRAGMENT_SHADER, fragmentShader, fragmentShaderSource);
        if(!isVertexShader || !isFragmentShader) return false;
        if(initProgram()) {
            initTypeTable();
            initVariables(vertexShaderSource, fragmentShaderSource);
            return true;
        }
        return false;
    }


    const std::string& Shader::getError() const {
        return error;
    }


    void Shader::use() const {
        glUseProgram(program);
    }


    const int &Shader::getUniform(const std::string &name) const {
        return 0;
    }


    mhy::Shader::~Shader() {
        glDeleteProgram(program);
    }

    bool Shader::initShader(GLenum type, unsigned int& shader, const std::string& source) {
        const char* c_source { source.c_str() };
        shader = glCreateShader(type);
        glShaderSource(shader, 1, &c_source, nullptr);
        glCompileShader(shader);
        int success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(success) return true;
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        error = "SHADER::ERROR\n" + std::string(infoLog);
        return false;
    }

    void Shader::initTypeTable() {
        typeTable["vec2"] = VEC2;
        typeTable["vec3"] = VEC3;
        typeTable["vec4"] = VEC4;
        typeTable["float"] = FLOAT;
        typeTable["mat3"] = MAT3;
        typeTable["mat4"] = MAT4;
        typeTable["sampler2D"] = SAMPLER2D;
    }

    /**
     * This function initialize a shader program
     * @param vertexShader is the compiled vertexShader
     * @param fragmentShader is the compiled fragmentShader
     */
    bool Shader::initProgram() {
        program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);
        int success;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        if(success) return true;
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        error = "PROGRAM::ERROR\n" + std::string(infoLog);
        glDeleteProgram(program);
        return false;
    }


    /**
     * @experimental
     * @todo To be reimplemented
     * @param vertexShaderSource
     * @param fragmentShaderSource
     */
    void Shader::initVariables(const std::string& vertexShaderSource, const std::string& fragmentShaderSource) {
        std::stringstream source{ vertexShaderSource + "\n" + fragmentShaderSource };
        std::string str;
        auto split = [](const std::string& str, const std::string& delimeter) -> std::vector<std::string> {
            auto cStr{ str };
            auto index{ cStr.find_first_of(delimeter) };
            auto storage = cStr.substr(0, index);
            cStr = cStr.substr(index + 1, cStr.size());
            index = cStr.find_first_of(delimeter);
            auto type = cStr.substr(0, index);
            cStr = cStr.substr(index + 1, cStr.size());
            auto name = cStr.substr(0, cStr.size() - 1);
            return { storage, type, name };
        };

        while(std::getline(source, str)) {
            if(str.starts_with("uniform")) {
                auto splitted = split(str, " ");
                auto location{ glGetUniformLocation(program, splitted[2].c_str()) };
                uniforms[splitted[2]] = { typeTable[splitted[1]], location };
            }
        } // while ends
    }


    template<typename T>
    void Shader::setUniform(const std::string &name, T *data, GLsizei count) {
        auto attr { uniforms[name] };
        switch(attr.first) {
            case FLOAT:
                glUniform1fv(attr.second, count, data);
                break;
            case VEC2:
                glUniform2fv(attr.second, count, data);
                break;
            case VEC3:
                glUniform3fv(attr.second, count, data);
                break;
            case VEC4:
                glUniform4fv(attr.second, count, data);
                break;
            case MAT4:
                glUniformMatrix4fv(attr.second, count, false, data);
                break;
            case SAMPLER2D:
                glUniform1iv(attr.second, count, data);
                break;
        }
    }


    Texture::Texture(const char *imgPath, int internalFormat, int destFormat, GLenum type, bool useMipmap, int mipLevel, int border){
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        unsigned char* img = stbi_load(imgPath, &width, &height, &nrChannel, 0);
        setError("");
        if(img) {
            _unit = unit++;
            glTexImage2D(GL_TEXTURE_2D, mipLevel, internalFormat, width, height, border, destFormat, type, img);
            if(useMipmap) glGenerateMipmap(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, 0);
        } else {
            setError("Texture Failed to load. Please make sure the path is correct");
        }
    }


    void Texture::setWrap(int s, int t) const {
        bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t);
        unbind();
    }


    void Texture::setFilter(int min, int mag) const {
        bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
        unbind();
    }


    void Texture::bind() const {
        glActiveTexture(GL_TEXTURE0 + _unit);
        glBindTexture(GL_TEXTURE_2D, texture);
    }


    void Texture::unbind() const {
        glBindTexture(GL_TEXTURE_2D, 0);
    }


    void Texture::setError(const std::string& msg) {
        error = msg;
    }


    const int& Texture::getWidth() const {
        return width;
    }


    const int& Texture::getHeight() const {
        return height;
    }


    const int& Texture::getChannel() const {
        return nrChannel;
    }


    const int& Texture::getUnit() const {
        return _unit;
    }


    const std::string& Texture::getError() const {
        return error;
    }

    int Texture::unit = 0;

}
