//
//  Framework for a raytracer
//  File: plane.h
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

#ifndef BOX_H_115209AE
#define BOX_H_115209AE

#include "object.h"

class Plane : public Object
{
public: 
    // Plane described by:
    // - normal, defining the plane's orientation
    // - to, the plane's distance from the world origin 
    Plane(Point normal,double to) : normal(normal), to(to) { }

    virtual Hit intersect(const Ray &ray);

    virtual double near(Triple eye);

    virtual double far(Triple eye);

    const Point normal;
    const double to;
};


#endif /* end of include guard: BOX_H_115209AE */
