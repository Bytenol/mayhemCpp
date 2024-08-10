#include "../include/mayhem/core.hpp"

std::string mhy::_logs;


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


mhy::img_ptr mhy::loadImage(const std::string& path) {
    std::unique_ptr<Image> img(new Image{});

    auto ii = path.c_str();
    img->data = stbi_load(ii, &img->width, &img->height, &img->channel, 0);
    if(img->data == nullptr) {
        setInfoLog(std::string { "Failed to load image: " } + stbi_failure_reason() );
        return nullptr;
    }

    return img;
}


mhy::buff_ptr mhy::createBuffer(BufferType type)
{
    std::unique_ptr<Buffer> vao(new Buffer{});
    vao->type = type;

    switch(type) {
        case BufferType::VERTEX_ARRAY_OBJECT:
            glGenVertexArrays(1, &vao->buffer);
            break;

        case BufferType::VERTEX_BUFFER_OBJECT:
        case BufferType::ELEMENT_ARRAY_OBJECT:
            glGenBuffers(1, &vao->buffer);
            bindBuffer(vao);
            break;

        default:
            throw mhy::InvalidBufferException("Cannot create an instance of an invalid buffer at " + std::to_string(__LINE__));
    }
    return vao;
}


void mhy::bindBuffer(buff_ptr& o)
{
    switch(o->type) {
        case BufferType::VERTEX_ARRAY_OBJECT:
            glBindVertexArray(o->buffer);;
            break;
        case BufferType::VERTEX_BUFFER_OBJECT:
            glBindBuffer(GL_ARRAY_BUFFER, o->buffer);
            break;
        case BufferType::ELEMENT_ARRAY_OBJECT:
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, o->buffer);
            break;
        default:
            throw mhy::InvalidBufferException("Cannot bind to an invalid buffer instance at " + std::to_string(__LINE__));
    }
}

void mhy::bindBuffer(BufferType type)
{
    switch(type) {
        case BufferType::VERTEX_ARRAY_OBJECT:
            glBindVertexArray(0);
            break;
        case BufferType::VERTEX_BUFFER_OBJECT:
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            break;
        case BufferType::ELEMENT_ARRAY_OBJECT:
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            break;
        default:
            throw mhy::InvalidBufferException("Cannot unbind from an invalid buffer instance" + std::to_string(__LINE__));
    }
}

mhy::Buffer::~Buffer()
{
    switch(type) {
        case BufferType::VERTEX_ARRAY_OBJECT:
            glDeleteVertexArrays(1, &buffer);
            break;

        case BufferType::VERTEX_BUFFER_OBJECT:
        case BufferType::ELEMENT_ARRAY_OBJECT:
            glDeleteBuffers(1, &buffer);
            break;

        default:
            setInfoLog("There may be some dangling buffers not deleted");
    }
}
