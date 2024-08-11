#ifndef __BYTENOL_STOOD_VERLY_H__
#define __BYTENOL_STOOD_VERLY_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <vector>


namespace vly {

    struct Point;
    struct Stick;

    std::vector<Point> _points;
    std::vector<Stick> _sticks;

    struct Point {
        float x;
        float y;
    };


    struct Stick {
        Point* p1;
        Point* p2;
        float length;
    };


}

#ifdef __cplusplus
}
#endif

#endif 