#ifndef ___BYTENOL__OPENGL_COOKLIB_MAYHEM__SHAPES__HPP__
#define ___BYTENOL__OPENGL_COOKLIB_MAYHEM__SHAPES__HPP__

#include "vector.hpp"
#include "exception.hpp"

#include <vector>


static std::vector<float> rectGeometry {
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,

    0.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f
};


namespace mhy {

    class Drawable {

        std::vector<float> vertexData;

        public:
            static std::vector<Drawable*> all;

            Vector3 position, size;

            Drawable();
    };

    
    class Rectangle {

        public:
            Vector3 position, size;
            float rotation = 0.0f;

            Rectangle() = default;

            Rectangle(const Vector3& position, const Vector3& size, bool isStatic = true);

            void setPosition(const Vector3& pos);

            void setSize(const Vector3& s);

            void setColor(const std::vector<float> color);

            std::vector<float> getData();

        private:
            bool isStatic = true;
            
    };

    
};

#endif 