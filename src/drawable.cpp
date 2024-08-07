#include "../include/mayhem/drawable.hpp"

#include <iostream>

using namespace mhy;


Rectangle::Rectangle(const Vector3& position, const Vector3& size, bool isStatic) {
    this->position = position;
    this->size = size;
    this->isStatic = isStatic;
}


void Rectangle::setPosition(const Vector3& pos) {
    position = pos;
}


void Rectangle::setSize(const Vector3& s) {
    size = s;
}


void Rectangle::setColor(const std::vector<float> color) {
    throw NotImplementedException();
}


std::vector<float> Rectangle::getData() {
    return {
        -0.5f, -0.5f, position.x, position.y, size.x, size.y, rotation,
        0.5f, -0.5f, position.x, position.y, size.x, size.y, rotation,
        0.5f, 0.5f, position.x, position.y, size.x, size.y, rotation,

        -0.5f, -0.5f, position.x, position.y, size.x, size.y, rotation,
        0.5f, 0.5f, position.x, position.y, size.x, size.y, rotation,
        -0.5f, 0.5f, position.x, position.y, size.x, size.y, rotation,
    };
}