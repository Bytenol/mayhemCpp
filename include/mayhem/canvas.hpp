#ifndef ___BYTENOL__OPENGL_COOKLIB_MAYHEM__CANVAS__HPP__
#define ___BYTENOL__OPENGL_COOKLIB_MAYHEM__CANVAS__HPP__

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "exception.hpp"
#include "drawable.hpp"
#include "shader.hpp"

#include <vector>
#include <string>
#include <chrono>
#include <map>


namespace mhy {


    /**
     * @brief An abstract class for getting GLFW window on the screen. The virtual classes
     * onEnter, onUpdate and onDraw must be implemented in the child class.
     * 
     * @todo Make setTitle() a static method
     */
    class Canvas {

        
        static float width, height;

        protected:

            using window_type = GLFWwindow;
            using window_ptr = window_type*;

            static void setWidth(const float& w);

            static void setHeight(const float& h);

            static const float& getWidth();

            static const float& getHeight();

            inline void setTitle(const std::string& t);

            inline float getElapsedTime() const;


        public:

            /**
             * @brief Construct a new Canvas object
             * 
             * @param w is the width of the canvas
             * @param h is the height of the canvas
             * @param title is the title of the canvas
             */
            Canvas(const float& w = 300.0f, const float& h = 150.0f, const std::string& title = "Untitled");

            /**
             * @brief Starts the canvas loop
             * 
             * @return true if there is no error
             * @return false if there is any error
             */
            void start();


            ~Canvas();


        protected:

            /**
             * @brief Set the Version of OpenGL. The minimum supported version is Opengl3.3
             * and that is what the canvas defaults to.
             * 
             * @param major is the major version number
             * @param minor is the minor version number
             */
            void setVersion(const int& major = 3, const int& minor = 3);


            void createVAO(const std::string& name);

            void bindVAO(const std::string& name);

            void createVBO(const std::string& name, GLsizeiptr size, const void* data, GLenum usage = GL_STATIC_DRAW);

            void bindVBO(const std::string& name);

            Rectangle createRect();

            /**
             * @brief This method is invoked just after the window opened. Initialization can be done here
             * 
             * @param window is the pointer to the GLFWwindow's object
             */
            virtual void onEnter(window_ptr window) = 0;


            /**
             * @brief This function is invoked on everyFrame
             * 
             * @param elapsedTime is the timeElapsed in seconds since when start() is called
             */
            virtual void onUpdate(float elapsedTime) = 0;


            /**
             * @brief This method is invoked on everyFrame
             * 
             * @param window is the pointer to GLFWwindow's object
             */
            virtual void onDraw(window_ptr window) = 0;


            /**
             * @brief This method is invoked just after the window closes. Every cleanup should be done here
             * 
             * @param window is the pointer to GLFWwindow's object
             */
            virtual void onExit(window_ptr window) { };


            //NOT IMPLEMENTED
            void addShader(const std::string& name, const Shader& shader, const std::vector<std::string>& locations);

            //NOT IMPLEMENTED
            void addShader(const std::string& name, const std::string& vertexShaderSource, 
                const std::string& fragmentShaderSource, const std::vector<std::string>& locations);

            //NOT IMPLEMENTED
            void addShader(const std::string& name, const std::string& vertexShaderSource, 
                const std::string& fragmentShaderSource, const std::string& geometryShaderSource, const std::vector<std::string>& locations);

            //NOT IMPLEMENTED
            void setCurrentShader(const std::string& name);

            //NOT IMPLEMENTED
            unsigned int getShaderLocation(const std::string& name);


        private:

            window_ptr window;

            std::string title;

            std::map<std::string, unsigned int> VAO, VBO;

            std::vector<Drawable> drawables;

            int version[2]{ 3, 3 };

            void initGlfWindow();

            float elapsedTime = 0;

            std::chrono::time_point<std::chrono::high_resolution_clock> timeStarted;

            std::string getUpdatedTitle(const float& elapsedTime) const;

    };


    inline void Canvas::setTitle(const std::string& t) {
        title = t;
    }


    inline float Canvas::getElapsedTime() const {
        return elapsedTime;
    }


    inline const float& Canvas::getWidth() {
        return width;
    }


    inline const float& Canvas::getHeight() {
        return height;
    }


    inline void Canvas::setWidth(const float& w) {
        width = w;
    }


    inline void Canvas::setHeight(const float& h) {
        height = h;
    }

}

#endif