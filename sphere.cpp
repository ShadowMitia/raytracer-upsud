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
#include "image.h"
#include <string.h>
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
  if(rprime > r*r + 1.0 || rprime < r*r - 1.0){
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
  double d = fabs(eye.z - position.z);
  return  d - r;
}

double Sphere::far(Triple eye)
{
  double d = fabs(eye.z - position.z);
  return d + r;
}


Color Sphere::mapping(Image *texture, Point hit)
{
    double u;
    Vector vp = hit - position;
    vp.normalize();

    //lattitude angle range 0-pi 
    double phi = (vn.dot(vp));
    phi = acos(phi);
    double v = phi / M_PI;

    //longiture angle on equator range 0-2pi
    Vector vpp = vn.cross(vp).cross(vn);
    vpp.normalize();
    double theta =  getAngle ( vpp , ve , vne) ;
    u = theta / ( 2 * M_PI);

    Color color_res = texture->colorAt(u,v);
    return color_res;
}

Vector rotate(Vector old, double ang, Vector dir)
{
   Vector t1 = cos(ang) * old;
   Vector t2 = (1 - cos(ang)) * old.dot(dir) * dir;
   Vector t3 = sin(ang) * dir.cross(old);
   Vector rotated = t1 + t2 + t3 ;
	
   return rotated;	
}

double Sphere::getAngle ( Vector v , Vector base , Vector normal ) 
{
   double a = v.dot(base);
   a = acos(a);
   double s = v.dot(normal);
   if ( s < 0 ){
     a = ( 2 * M_PI ) - a ;
   }
   return ( a ) ;
}


void Sphere::computeVeVn()
{
   Vector XVec = Triple(1.0, 0.0, 0.0);
   Vector YVec = Triple(0.0, 1.0, 0.0);
   Vector ZVec = Triple(0.0, 0.0, 1.0);
   vn = ZVec;
   ve = XVec;
   vne = YVec ;
  
   if ( angle != 0 ) {
     ve = rotate(ve, ((angle * M_PI ) / 180), ZVec);
     ve.normalize();
   }
   if(vec.x != 0.0 || vec.y != 0.0 ){
     vec.normalize() ;
     vne = ZVec.cross(vec) ;
     vne.normalize() ;
     vn = rotate(vn, getAngle ( vec , ZVec , vne) , vne);
     vn.normalize();
  }
  vne = vn.cross(ve);
  vne.normalize();
  ve = vne.cross(vn);
}

Color Sphere::UVMapping(Point hit){

    double u;
    Vector vp = hit - position;
    vp.normalize();

    //lattitude angle range 0-pi 
    double phi = (vn.dot(vp));
    phi = acos(phi);
    double v = phi / M_PI;

    //longiture angle on equator range 0-2pi
    Vector vpp = vn.cross(vp).cross(vn);
    vpp.normalize();
    double theta =  getAngle ( vpp , ve , vne) ;
    u = theta / ( 2 * M_PI);

    return Color(u, 0.0, v);
}


Color Sphere::getColor(Point hit)
{

   if(material->texture == ""){
      return material->color;
   }else{
      if(!imageLoaded){
	 if(strcmp ( material->texture.c_str() , "UV") == 0){
	   Vector XVec = Triple(1.0, 0.0, 0.0);
	   Vector YVec = Triple(0.0, 1.0, 0.0);
	   Vector ZVec = Triple(0.0, 0.0, 1.0);
	   vn = YVec;
	   ve = XVec;
	   vne = -ZVec ;
         }else{		
	   textureImage = new Image(material->texture.c_str());
	   if(textureImage->width() == 0){
	      std::cout << "!!! Impossible de lire l'image" << material->texture.c_str() << "\n";
	   }
	   computeVeVn();
        }
        imageLoaded = true;
      }
      if(strcmp ( material->texture.c_str() , "UV") == 0){
         return UVMapping(hit);
      }else{
	 return mapping(textureImage, hit);
      }
   }
}
