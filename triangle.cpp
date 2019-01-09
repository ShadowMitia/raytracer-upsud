#include "triangle.h"
#include <iostream>
#include <math.h>

/************************** Triangle **********************************/

Hit Triangle::intersect(const Ray &ray)
{
  double t = 0;

  //face p1, p2, p3
  t = computeDistance(p1, N, ray);
  if(!PointInTriangle(ray.O + ray.D * t, p1, p2, p3)){
  	return Hit::NO_HIT();
  }

  if(t <= 0){
	  return Hit::NO_HIT();
  }

  return Hit(t,N);
}

bool Triangle::SameSide(Point p1,Point p2, Point a,Point b){
   Point  cp1 = (b-a).cross(p1-a);
   Point  cp2 = (b-a).cross(p2-a);
    if( cp1.dot(cp2) >= 0 ){
	 return true;
    }else{
	 return false;
    }
}

bool Triangle::PointInTriangle(Point p,Point a, Point b, Point c){
    if( SameSide(p,a, b,c) && SameSide(p,b, a,c) && SameSide(p,c, a,b)){
	return true;
    }else{
	return false;
    }
}

double Triangle::computeDistance(Point p, Vector N, const Ray &ray){
   double t;
   if(ray.D.dot(N) == 0){
     return 0;
   }
   t = (p.dot(N) - ray.O.dot(N))/ray.D.dot(N);
   return t;
}

void Triangle::InitTriangle() {
  //Normal of p1, p2, p3
  N = computeFaceNormal(p1, p2, p3) ;
  N.normalize();
}

Vector Triangle::computeFaceNormal(Point p1,Point p2, Point p3){
  Vector A = p2 - p1;
  Vector B = p3 - p2;
  Vector N = Triple(A.y*B.z-A.z*B.y, A.z*B.x-A.x*B.z, A.x*B.y-A.y*B.x) ;
  return N;

}
double Triangle::near(Triple eye)
{
   double d = 0;
   return d;
}

double Triangle::far(Triple eye)
{
   double d = 0;
   return d;
}

Color Triangle::getColor(Point hit, Point normal)
{
  return material->color;
}
