#ifndef CAMERA_H_TWMNT2EJ
#define CAMERA_H_TWMNT2EJ

#include <iostream>
#include "triple.h"

class Camera
{
public:
    Camera(Point eye, Point center, Point up, Point viewSize) : eye(eye), center(center), up(up) , viewSize(viewSize) { }

    Triple eye;
    Triple center;
    Triple up;
    Triple viewSize;
};

#endif /* end of include guard: CAMERA_H_TWMNT2EJ */
