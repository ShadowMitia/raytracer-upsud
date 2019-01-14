//
//  Framework for a raytracer
//  File: plane.cpp
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Authors:
//    Maarten Everts
//    Jasper van de Gronde
//    Virginie Touchais
//    Dimitri Belopopsky
//
//  This framework is inspired by and uses code of the raytracer framework of
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html
//

#include "plane.h"
#include <iostream>
#include <math.h>

/************************** Plane **********************************/

Hit Plane::intersect(const Ray &ray)
{
  Vector N = normal;
  N.normalize();
  /*
   * Let's say the point Po is the intersection point between the plane and the line passing by the world's origin with the normal vector.
   * The point p, the intersection point between the plane and the ray.
   * Po != p and since both points belong to the plane:
   *	 (1) (p - Po).dot(normal) = 0
   * We can also define p by:
   *	 (2) p = ray.O + t * ray.D
   * In the same way Po can be defined by:
   *   (3) Po = [0, 0, 0] + to * normal = to * normal;
   * When we insert (2) and (3) in (1) we get:
   *	 ( ray.O + t * ray.D - to * normal ).dot(normal) = 0
   *   => ray.O.dot(normal) + (t * ray.D).dot(normal) - (to * normal).dot(normal) = 0
   *   => ray.O.dot(normal) + (t * ray.D).dot(normal) - to * (normal).dot(normal) = 0
   *   => ray.O.dot(normal) + (t * ray.D).dot(normal) - to = 0
   *   => ray.O.dot(normal) + t * ray.D.dot(normal) - to = 0
   *   => t * ray.D.dot(normal) = to - ray.O.dot(normal)
   *   => t = (to - ray.O.dot(normal)) / ray.D.dot(normal)
   */

  //distance of the eye origin to the intersection point
  double t = (to - ray.O.dot(N))/(ray.D.dot(N));
  return Hit(t,N);
}

double Plane::near(Triple eye)
{
  double d = abs(eye.z - normal.z);
  return  d - to;
}

double Plane::far(Triple eye)
{
  double d = abs(eye.z - normal.z);
  return d + to;
}

Color Plane::getColor(Point hit)
{
  if (material->showUV) {
    return UVMapping(hit);
  }

  return material->color;
}
