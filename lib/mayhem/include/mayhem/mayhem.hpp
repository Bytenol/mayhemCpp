#ifndef __MAYHEM__LIB_HPP__
#define __MAYHEM__LIB_HPP__

#include <mayhem/pch.hpp>


namespace mhy {

    class GLRenderer {

    public:
        GLRenderer(const GLRenderer& o) = delete;
        GLRenderer& operator=(const GLRenderer& o) = delete;
        GLRenderer(int width = 350, int height = 100, std::string title = "Untitled");
        bool init();
        const std::string& getError() const;
        void start();
        ~GLRenderer();

    protected:
        std::default_random_engine random;

        // setters
        void setVersion(const int& major, const int& minor);
        void setSize(const int& w, const int& h);
        void setTitle(const std::string& t);
        void setError(const std::string& t);

        // getters 
        GLFWwindow* getContext() const;
        const int& getVersionMajor() const;
        const int& getVersionMinor() const;
        const int& getWidth() const;
        const int& getHeight() const;
        const std::string& getTitle() const;
        const float& getElapsedTime() const;

        // abstract methods 
        virtual void onStart(){ };
        virtual void onUpdate(float dt){ };
        virtual void onDraw(){ };
        virtual void onResize(){ };
        virtual void onClose(){ };

    private:
        GLFWwindow* window{  };
        int version[2]{ 3, 3 };     // opengl3.3 by default
        int size[2]{ 300, 150 };    // size of the window
        int currentSize[2]{ 300, 150 };
        std::string info[2]{ };     // title, error
        std::chrono::_V2::high_resolution_clock::time_point lastTime, elapsedTimeStarted;
        float elapsedTime;

    };



    class Shader {

    public:
        unsigned int program{ };
        Shader() = default;
        bool loadFromString(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
        void use() const;
        const std::string& getError() const;
        template<typename T>void setUniform(const std::string& name, T* data, GLsizei count = 1);
        const int& getUniform(const std::string& name) const;
        ~Shader();

    private:
        enum TYPE {
            FLOAT,
            VEC2,
            VEC3,
            VEC4,
            MAT3,
            MAT4,
            SAMPLER2D,
        };

        std::string error;
        unsigned int vertexShader, fragmentShader;
        std::map<std::string, std::pair<TYPE, int>> uniforms;
        std::map<std::string, int> attributes;
        std::map<std::string, TYPE> typeTable;
        void initTypeTable();
        bool initShader(GLenum type, unsigned int& shader, const std::string& source);
        bool initProgram();
        void initVariables(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);

    };


    class Texture {

    public:
        Texture() = default;
        Texture(const char* imgPath, int internalFormat, int destFormat, GLenum type, bool useMipmap = false, int mipLevel = 0, int border = 0);
        void setWrap(int s, int t) const;
        void setFilter(int min, int mag) const;
        void bind() const;
        void unbind() const;
        const std::string& getError() const;
        const int& getWidth() const;
        const int& getHeight() const;
        const int& getChannel() const;
        const int& getUnit() const;

    private:
        unsigned texture;
        int width, height, nrChannel, _unit;
        std::string error;
        void setError(const std::string& msg);

        static int unit;
    };

}

#endif