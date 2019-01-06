//
//  Framework for a raytracer
//  File: object.h
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

#ifndef OBJECT_H_AXKLE0OF
#define OBJECT_H_AXKLE0OF

#include "triple.h"
#include "light.h"

#include "material.h"

class Material;

class Object {
public:
    Material *material;

    virtual ~Object() { }

    virtual Hit intersect(const Ray &ray) = 0;

    virtual double near(Triple eye) = 0;

    virtual double far(Triple eye) = 0;

  virtual Color getColor(Point hit, Point normal) = 0;

  virtual Color UVMapping(Point hit) = 0;

  virtual Color mapping(Image *texture, Point hit) = 0;
};

#endif /* end of include guard: OBJECT_H_AXKLE0OF */
