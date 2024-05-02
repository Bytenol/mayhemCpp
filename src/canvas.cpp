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


    std::string Canvas::getUpdatedTitle(float elapsedTime) const {
        return title + "  -- Elapsed: " + std::to_string(static_cast<int>(elapsedTime)) + "s";
    }


    void Canvas::start() {

        initGlfWindow();
        onEnter(window);

        timeStarted = std::chrono::high_resolution_clock::now();        
        decltype(timeStarted) currentTime;
        std::chrono::duration<float> timeDiff;
        float elapsedTime;

        while (!glfwWindowShouldClose(window))
        {
            currentTime = std::chrono::high_resolution_clock::now();
            timeDiff = currentTime - timeStarted;
            elapsedTime = timeDiff.count();
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
        _allShaders.emplace(std::make_pair(name, shader));
        _allShaders.at(name).addLocation(locations);
        setCurrentShader(name);
    };


    void Canvas::addShader(const std::string& name, const std::string& vertexShaderSource, 
    const std::string& fragmentShaderSource, const std::vector<std::string>& locations) 
    {
        Shader shader;
        shader.loadFromString(vertexShaderSource, fragmentShaderSource);
        addShader(name, shader, locations);
    };


    void Canvas::addShader(const std::string& name, const std::string& vertexShaderSource, 
    const std::string& fragmentShaderSource, const std::string& geometryShaderSource, const std::vector<std::string>& locations) 
    {
        Shader shader;
        shader.loadFromString(vertexShaderSource, fragmentShaderSource, geometryShaderSource);
        addShader(name, shader, locations);
    };


    void Canvas::setCurrentShader(const std::string& name) {
        if(!(_allShaders.contains(name)))
            throw std::runtime_error("Error while shader is trying to get '" + name + "'");
        currShader = name;
        _allShaders.at(currShader).use();
    }


    unsigned int Canvas::getShaderLocation(const std::string& name) {
        return _allShaders.at(currShader).getLocation(name);
    }


    Canvas::~Canvas() {
        glfwDestroyWindow(window);
    }

}