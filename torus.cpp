//
//  Framework for a raytracer
//  File: sphere.cpp
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

#include "torus.h"
#include <iostream>
#include <math.h>

/************************** Sphere **********************************/

Hit Torus::intersect(const Ray &ray)
{
  /****************************************************
   * RT1.1: INTERSECTION CALCULATION
   *
   * Given: ray, position, r, R
   * Sought: intersects? if true: *t
   *
   * Insert calculation of ray/torusintersection here.
   *
   * You have the torus' center (C), tube radius (r) and torus radius (R) as well as
   * the ray's origin (ray.O) and direction (ray.D).
   *
   * If the ray does not intersect the torus, return Hit::NO_HIT().
   * Otherwise, return an instance of Hit() with the distance of the
   * intersection point from the ray origin as t and the normal as N (see example).
   ****************************************************/

  Vector OC = (position - ray.O);
  //Distance between C and ray
  double distance = (OC - (  (OC.dot(ray.D)/ray.D.dot(ray.D)) *ray.D ) ).length();

  if (distance > r) {
    return Hit::NO_HIT();
  }

  //Distance of the projection of C on the ray from the ray origin
  double tco = sqrt(OC.dot(OC) - distance * distance);
  //Distance of the projection of C on the ray from the intersection point
  double tcp = sqrt(r * r - distance * distance);
  //Distance of the intersection point from the ray origin
  double t = tco - tcp;

  /****************************************************
   * RT1.2: NORMAL CALCULATION
   *
   * Given: t, C, r, R
   * Sought: N
   *
   * Insert calculation of the torus' normal at the intersection point.
   ****************************************************/

  //Intersection point P = O + OP, (OP = t * ray.D)
  Vector P = ray.O + t * ray.D;
  //Vector CP = N
  Vector N = P - position;
  N.normalize();

  return Hit(t,N);
}

double Torus::near(Triple eye) {
  double d = abs(eye.z - position.z);
  return  d - r;
}

double Torus::far(Triple eye) {
  double d = abs(eye.z - position.z);
  return d + r;
}
