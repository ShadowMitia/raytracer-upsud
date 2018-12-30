//
//  Framework for a raytracer
//  File: sphere.h
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Authors:
//    Maarten Everts
//    Jasper van de Gronde
//
//  This framework is inspired by and uses code of the raytracer framework of
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html
//

#ifndef SPHERE_H_115209AE
#define SPHERE_H_115209AE

#include "object.h"
#include "image.h"

class Sphere : public Object
{
public:
    Sphere(Point position_, double r_, double angle_, Triple vec_) : position(position_), r(r_), angle(angle_), vec(vec_) { }

    virtual Hit intersect(const Ray &ray);

    virtual double near(Triple eye);

    virtual double far(Triple eye);

    const Point position;
    const double r;
    double angle;
    Triple vec;
    Vector ve, vn, vne ;
    Image *textureImage;
    bool imageLoaded = false;

    virtual Color mapping(Image *texture, Point hit);
    virtual Color getColor(Point hit);
    virtual Color UVMapping(Point hit);
    virtual double getAngle (Vector v , Vector base, Vector normal); 
    virtual void computeVeVn();
};

#endif /* end of include guard: SPHERE_H_115209AE */
