#ifndef ___BYTENOL__OPENGL_COOKLIB_MAYHEM__SHADER__HPP__
#define ___BYTENOL__OPENGL_COOKLIB_MAYHEM__SHADER__HPP__

#include "exception.hpp"

#include <string>
#include <vector>
#include <map>


namespace mhy {


    class Shader {

        public:

            Shader() = default;

            void loadFromString(const std::string& vertexShaderSource, const std::string& fragmentShaderSource, const std::string& geometryShaderSource = "");

            void loadFromFile(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& geometryShaderPath);

            void use();

            const unsigned int& getProgram();

            void addLocation(const std::string& name);

            void addLocation(const std::vector<std::string>& lst);

            const std::string& getVertexShaderSource() const;

            const std::string& getFragmentShaderSource() const;

            const std::string& getGeometryShaderSource() const;

            inline unsigned int getLocation(const std::string& name) const;

            ~Shader();


        private:


            bool isInitialized = false;

            unsigned int program;

            std::string vSource, fSource, gSource;

            std::map<std::string, unsigned int> location;

            void initProgram();
    };


    inline const unsigned int& Shader::getProgram() { 
        return program; 
    };


    inline unsigned int Shader::getLocation(const std::string& name) const {
        try {
            return location.at(name);
        } catch(std::exception& e) {
            throw std::runtime_error("Error while shader is trying to get the location for " + name);
        }
    }


    inline const std::string& Shader::getVertexShaderSource() const {
        return vSource;
    }


    inline const std::string& Shader::getFragmentShaderSource() const {
        return fSource;
    }


    inline const std::string& Shader::getGeometryShaderSource() const {
        return gSource;
    }

}

#endif 