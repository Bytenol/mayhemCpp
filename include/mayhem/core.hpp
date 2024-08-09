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


namespace mhy {

    extern std::string _logs;

    class Vao;
    struct Vbo;
    struct Ibo;
    struct Image;
    struct Texture2D;

    using vao_ptr = std::unique_ptr<Vao>;

    std::string getBackDirectory(std::string currPath, int backCount);

    std::unique_ptr<Image> loadImage(const std::string& path);

    vao_ptr createVao();
    void bindVao(vao_ptr o);
    void unbindVao();

    /**
     * Vertex array objects
     */
    class Vao {

        friend vao_ptr createVao();
        friend void bindVao(vao_ptr o);

        public:
            explicit Vao() = default;
            ~Vao();

        private:
            unsigned int buffer;
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


    inline void setInfoLog(const std::string& msg) {
        _logs = msg;
    };

    inline const std::string& getInfoLog() {
        return _logs;
    };

};


#endif 