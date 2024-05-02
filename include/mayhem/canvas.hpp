#ifndef ___BYTENOL__OPENGL_COOKLIB_MAYHEM__CANVAS__HPP__
#define ___BYTENOL__OPENGL_COOKLIB_MAYHEM__CANVAS__HPP__

#include "../../deps/glad/include/glad/glad.h"
#define GLFW_INCLUDE_NONE
#include "../../deps/glfw-3.3.8/include/GLFW/glfw3.h"
#include "shader.hpp"
#include "exception.hpp"

#include <vector>
#include <string>
#include <chrono>
#include <map>
#include <utility>


namespace mhy {


    /**
     * @brief An abstract class for getting GLFW window on the screen. The virtual classes
     * onEnter, onUpdate and onDraw must be implemented in the child class.
     * 
     * @todo Make setTitle() a static method
     */
    class Canvas {

        private:
            static float width;

            static float height;


        protected:

            static void setWidth(const float& w);

            static void setHeight(const float& h);

            static float& getWidth();

            static float& getHeight();


        private:

            GLFWwindow* window;

            std::string title;

            std::string error;

            std::chrono::time_point<std::chrono::high_resolution_clock> timeStarted;

            std::map<std::string, Shader> _allShaders;

            std::string currShader;

            int version[2]{ 3, 3 };

            void initGlfWindow();

            std::string getUpdatedTitle(float elapsedTime) const;


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

            using window_type = GLFWwindow;
            using window_ptr = GLFWwindow*;

            /**
             * @brief Set the Version of OpenGL. The minimum supported version is Opengl3.3
             * and that is what the canvas defaults to.
             * 
             * @param major is the major version number
             * @param minor is the minor version number
             */
            void setVersion(const int& major = 3, const int& minor = 3);

            void addShader(const std::string& name, const Shader& shader, const std::vector<std::string>& locations);

            void addShader(const std::string& name, const std::string& vertexShaderSource, 
                const std::string& fragmentShaderSource, const std::vector<std::string>& locations);

            void addShader(const std::string& name, const std::string& vertexShaderSource, 
                const std::string& fragmentShaderSource, const std::string& geometryShaderSource, const std::vector<std::string>& locations);

            void setCurrentShader(const std::string& name);

            unsigned int getShaderLocation(const std::string& name);

            inline void setTitle(const std::string& t);

            virtual void onEnter(window_ptr window) = 0;

            virtual void onUpdate(float elapsedTime) = 0;

            virtual void onDraw(window_ptr window) = 0;

            virtual void onExit(window_ptr window) { };
    };


    inline void Canvas::setTitle(const std::string& t) {
        title = t;
    }


    inline float& Canvas::getWidth() {
        return width;
    }


    inline float& Canvas::getHeight() {
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