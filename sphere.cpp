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

#include "sphere.h"
#include <iostream>
#include <math.h>

/************************** Sphere **********************************/

Hit Sphere::intersect(const Ray &ray)
{
  /****************************************************
   * RT1.1: INTERSECTION CALCULATION
   *
   * Given: ray, position, r
   * Sought: intersects? if true: *t
   *
   * Insert calculation of ray/sphere intersection here.
   *
   * You have the sphere's center (C) and radius (r) as well as
   * the ray's origin (ray.O) and direction (ray.D).
   *
   * If the ray does not intersect the sphere, return Hit::NO_HIT().
   * Otherwise, return an instance of Hit() with the distance of the
   * intersection point from the ray origin as t and the normal ad N (see example).
   ****************************************************/

  // place holder for actual intersection calculation


  Vector OC = (position - ray.O);
  //Distance between C and ray
  double distance = (OC - (  (OC.dot(ray.D)/ray.D.dot(ray.D)) *ray.D ) ).length();

  if (distance > r || distance < 0) {
    return Hit::NO_HIT();
  }
  //Distance of the projection of C on the ray from the ray origin
  double tco = sqrt(OC.dot(OC) - distance * distance);
  //Distance of the projection of C on the ray from the intersection point
  double tcp = sqrt(r * r - distance * distance);
  //Distance of the intersection point from the ray origin
  double t = tco - tcp;

  //To avoid same point as intersection
  if ( t < 1 ) { 
    return Hit::NO_HIT();
  }

  Vector CP = (ray.O + t * ray.D) - position;  
  double rprime = CP.dot(CP);
  //To avoid intersection point behind the ray.O
  if(rprime > r*r + 1 || rprime < r*r - 1){
       return Hit::NO_HIT();
  }
  /****************************************************
   * RT1.2: NORMAL CALCULATION
   *
   * Given: t, C, r
   * Sought: N
   *
   * Insert calculation of the sphere's normal at the intersection point.
   ****************************************************/

  //Intersection point P = O + OP, (OP = t * ray.D)
  Vector P = ray.O + t * ray.D;
  //Vector CP = N
  Vector N = P - position;
  N.normalize();

  return Hit(t,N);
}

double Sphere::near(Triple eye)
{
	double d = abs(eye.z - position.z);
	return  d - r;
}

double Sphere::far(Triple eye)
{
	double d = abs(eye.z - position.z);
	return d + r;
}
