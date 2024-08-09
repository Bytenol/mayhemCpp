#include "../include/mayhem/core.hpp"


std::string mhy::_logs;

/**
 * Gets the directory of a file by looping backward the tree
 * @param currPath is the starting directory
 * @param backCount is the number of parent tree to skip backwardly
 */
std::string mhy::getBackDirectory(std::string currPath, int backCount)
{
    auto cPath = std::filesystem::path(currPath).parent_path();

    while (backCount > 0)
    {
        cPath = std::filesystem::path(cPath).parent_path();
        backCount--;
    }
    
    return cPath;
}


/**
 * Load an image file.
 * @param path is the path to the image resource
 * @return true a unique_ptr to the image
 */
std::unique_ptr<mhy::Image> mhy::loadImage(const std::string& path) {
    std::unique_ptr<Image> img(new Image{});

    auto ii = path.c_str();
    img->data = stbi_load(ii, &img->width, &img->height, &img->channel, 0);
    if(img->data == nullptr) {
        setInfoLog(std::string { "Failed to load image: " } + stbi_failure_reason() );
        return nullptr;
    }

    return img;
}

std::unique_ptr<mhy::Vao> mhy::createVao()
{
    std::unique_ptr<Vao> vao(new Vao{});
    glGenVertexArrays(1, &vao->buffer);
    return vao;
}

void mhy::bindVao(vao_ptr o)
{
    glBindVertexArray(o->buffer);
}

void mhy::unbindVao()
{
    glBindVertexArray(0);
}

mhy::Vao::~Vao()
{
    glDeleteVertexArrays(1, &buffer);
}
