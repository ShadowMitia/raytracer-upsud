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

#ifndef PLANE_H_115209AE
#define PLANE_H_115209AE

#include "object.h"

class Plane : public Object
{
public:
    // Plane described by:
    // - normal, defining the plane's orientation
    // - to, the plane's distance from the world origin
    Plane(Point normal_,double to_) : normal(normal_), to(to_) { }

    virtual Hit intersect(const Ray &ray);

    virtual double near(Triple eye);

    virtual double far(Triple eye);

    const Point normal;
    const double to;

  virtual Color mapping(Image *texture, Point hit) override  {
    return Color(0, 0, 0);
  }
  virtual Color getColor(Point hit, Point normal) override;
  virtual Color UVMapping(Point hit) override {
    return Color(0, 0, 0);
  };
};


#endif /* end of include guard: PLANE_H_115209AE */
