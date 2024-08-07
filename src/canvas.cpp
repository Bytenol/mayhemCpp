#include "../include/mayhem/canvas.hpp"

namespace mhy {

    float Canvas::width = 300.0f;

    float Canvas::height = 150.0f;

    Canvas::Canvas(const float& w, const float& h, const std::string& title) {
        setWidth(w);
        setHeight(h);
        setTitle(title);
    }


    void Canvas::setVersion(const int& major, const int& minor) {
        if(major < 3) throw GlFailureException("The GL version is too low: Minimum is 3.3");

        version[0] = major;
        version[1] = minor;
    }


    void Canvas::createVAO(const std::string& name) {
        if(VAO.contains(name)) return;
        VAO[name] = 0;
        glGenVertexArrays(1, &(VAO[name]));
        glBindVertexArray(VAO[name]);
    }


    void Canvas::bindVAO(const std::string& name) {
        if(!VAO.contains(name)) 
            throw std::runtime_error("Vertex Array object with the name '" + name + "' does not exists");
        glBindVertexArray(VAO[name]);
    }


    void Canvas::createVBO(const std::string& name, GLsizeiptr size, const void* data, GLenum usage) {
        if(VBO.contains(name)) return;
        VBO[name] = 0;
        glGenBuffers(1, &(VBO[name]));
        glBindBuffer(GL_ARRAY_BUFFER, VBO[name]);
        glBufferData(GL_ARRAY_BUFFER, size, data, usage);
    }


    void Canvas::bindVBO(const std::string& name) {
        if(!VBO.contains(name))
            throw std::runtime_error("Vertex Buffer Object with the name '" + name + "' does not exists");
        glBindBuffer(GL_ARRAY_BUFFER, VBO[name]);
    }


    void Canvas::initGlfWindow() {
        if(!glfwInit()) {
            throw GlFailureException("Failed to initialize GLFW");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, version[0]);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, version[1]);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(static_cast<unsigned int>(width), static_cast<unsigned int>(height), "", nullptr, nullptr);
        if(!window) throw GlFailureException("Seems like the Opengl version provided is not supported by your hardware");

        glfwMakeContextCurrent(window);
        if(!gladLoadGL()) {
            throw GlFailureException("Glad is unable to load opengl");
        }

    }


    std::string Canvas::getUpdatedTitle(const float& elapsedTime) const {
        return title ;//+ "  -- Elapsed: " + std::to_string(elapsedTime / 1000.0f) + "s";
    }


    void Canvas::start() {

        initGlfWindow();

        onEnter(window);
        
        decltype(timeStarted) currentTime, deltaStarted;
        float deltaDiff;
        deltaStarted = std::chrono::high_resolution_clock::now();
        timeStarted = std::chrono::high_resolution_clock::now();
        

        while (!glfwWindowShouldClose(window))
        {
            currentTime = std::chrono::high_resolution_clock::now();
            elapsedTime = static_cast<float>((currentTime - timeStarted).count());  // in seconds
            deltaDiff = (currentTime.time_since_epoch().count() - deltaStarted.time_since_epoch().count()) / 1e9;
            deltaStarted = currentTime;
            
            glfwSwapBuffers(window);
            glfwPollEvents();
            const char* _title = getUpdatedTitle(elapsedTime).c_str();
            glfwSetWindowTitle(window, _title);
            glfwSetWindowSize(window, width, height);
            onUpdate(elapsedTime);
            onDraw(window);
        }
        
        onExit(window);
    }


    void Canvas::addShader(const std::string& name, const Shader& shader, const std::vector<std::string>& locations) {
        throw NotImplementedException();
    };


    void Canvas::addShader(const std::string& name, const std::string& vertexShaderSource, 
    const std::string& fragmentShaderSource, const std::vector<std::string>& locations) 
    {
       throw NotImplementedException();
    };


    void Canvas::addShader(const std::string& name, const std::string& vertexShaderSource, 
    const std::string& fragmentShaderSource, const std::string& geometryShaderSource, const std::vector<std::string>& locations) 
    {
        throw NotImplementedException();
    };


    void Canvas::setCurrentShader(const std::string& name) {
        throw NotImplementedException();
    }


    unsigned int Canvas::getShaderLocation(const std::string& name) {
       throw NotImplementedException();
    }


    Canvas::~Canvas() {

        for(auto& vbo: VBO) glDeleteBuffers(1, &(vbo.second));

        for(auto& vao: VAO) glDeleteVertexArrays(1, &(vao.second));

        glfwDestroyWindow(window);
    }

}