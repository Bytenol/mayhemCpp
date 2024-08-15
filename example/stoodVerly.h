#ifndef __BYTENOL_STOOD_VERLY_H__
#define __BYTENOL_STOOD_VERLY_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <vector>
#include <cmath>
#include <stdexcept>


namespace vly {

    struct Vector {
        float x, y, z;

        explicit Vector(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) {
            x = _x;
            y = _y;
            z = _z;
        };

        Vector operator+(const Vector& v) const {
            return Vector{ x + v.x, y + v.y, z + v.z };
        }

        Vector& operator+=(const Vector& v) {
            x += v.x;
            y += v.y;
            z += v.z;
            return *this;
        }

        Vector operator-(const Vector& v) const {
            return Vector{ x - v.x, y - v.y, z - v.z };
        }

        Vector& operator-=(const Vector& v) {
            x -= v.x;
            y -= v.y;
            z -= v.z;
            return *this;
        }

        Vector operator/(const float& s) {
            return Vector{ x / s, y / s, z / s };
        }

        Vector operator*(const float& s) {
            return Vector{ x * s, y * s, z * s };
        }

        inline float length() {
            return std::hypot(x, y, z);
        }

        Vector normalize() {
            auto l = length();
            if(l == 0.0f) throw std::runtime_error("Cannot normalize vector by zero");
            return Vector{ x / l, y / l, z / l };
        }
    };


}

#ifdef __cplusplus
}
#endif

#endif 