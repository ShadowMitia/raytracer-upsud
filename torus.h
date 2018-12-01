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

#ifndef TORUS_H
#define TORUS_H

#include "object.h"

class Torus : public Object
{
public:
  Torus(Point position, double r, double R) : position(position), r(r), R(R) {

  }

  const Point position;
  const double r;
  const double R;

  virtual Hit intersect(const Ray &ray) override;
  double near(Triple eye);
  double far(Triple eye);
};


#endif /* end of include guard: TORUS_H */
