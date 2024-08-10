/**
 * @file core.hpp
 * @date 9th Aug, 2024
 * 
 * @todo documentation
 * @todo createImage()
 * @todo hide image class api
 * @todo createTexture()
 */
#ifndef ___BYTENOL__CORE_MAYHEM_CPP__HPP__
#define ___BYTENOL__CORE_MAYHEM_CPP__HPP__

#include <string>
#include <memory>
#include <filesystem>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#ifndef STB_IMAGE_IMPLEMENTATION
    #include <stb_image/stb_image.h>
#endif

#include <mayhem/exception.hpp>


namespace mhy {

    extern std::string _logs;

    class Buffer;
    struct Image;
    struct Texture2D;

    using buff_ptr = std::unique_ptr<Buffer>;
    using img_ptr = std::unique_ptr<Image>;

    enum class BufferType {
        VERTEX_ARRAY_OBJECT,
        VERTEX_BUFFER_OBJECT,
        ELEMENT_ARRAY_OBJECT
    };

    /**
     * Gets the directory of a file by looping backward the tree
     * @param currPath is the starting directory
     * @param backCount is the number of parent tree to skip backwardly
     */
    std::string getBackDirectory(std::string currPath, int backCount);


    /**
     * Load an image file.
     * @param path is the path to the image resource
     * @return true an img_ptr to the image
     */
    img_ptr loadImage(const std::string& path);

    buff_ptr createBuffer(BufferType type);
    void bindBuffer(buff_ptr& o);
    void bindBuffer(BufferType type);

    /**
     * Buffer objects
     * by implementation vbo, ibo are binded at where they are created
     */
    class Buffer {

        friend buff_ptr createBuffer(BufferType type);
        friend void bindBuffer(buff_ptr& o);

        public:
            explicit Buffer() = default;
            ~Buffer();

        private:
            unsigned int buffer;
            BufferType type;
    };

    struct Image {
        int width;
        int height;
        int channel;
        unsigned char* data = nullptr;
    };

    struct Texture2D {
        std::unique_ptr<Image> image = nullptr;
    };


    inline void setInfoLog(const std::string& msg) { _logs = msg; };

    inline const std::string& getInfoLog() { return _logs; };

};


#endif 