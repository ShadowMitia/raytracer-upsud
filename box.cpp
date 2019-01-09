#include "box.h"
#include <iostream>
#include <math.h>

/************************** Box **********************************/

Hit Box::intersect(const Ray &ray)
{
  double t = 0;
  Vector N;
  double tmin = 0;
  Vector Nmin;

  //	Box faces order:
  //
  //    5 --------- 6
  //    / |       /|
  //   /  |      / |
  // 1 ---|----- 2 | 
  //   | 8/     |  /7
  //   | /      | /
  //   |/       |/
  //  4 -------- 3


  //face p1, p2, p3, p4
  t = computeDistance(p1, N1, ray);
  if(PointInQuad(ray.O + ray.D * t, p1, p2, p3, p4)){
  	if(newTmin(tmin, t)){
		tmin = t;
		Nmin = N1;
	}
  }
  //face p8, p7, p3, p4
  t = computeDistance(p8, N2, ray);
  if(PointInQuad(ray.O + ray.D * t, p8, p7, p3, p4)){
  	if(newTmin(tmin, t)){
		tmin = t;
		Nmin = N2;
	}
  
  }
  //face p1, p4, P8, p5
  t = computeDistance(p1, N3, ray);
  if(PointInQuad(ray.O + ray.D * t, p1, p4, p8, p5)){
   	if(newTmin(tmin, t)){
		tmin = t;
		Nmin = N3;
	}
 
  }
 
  //face p5, p6, p7, p8
  t = computeDistance(p5, N4, ray);
  if(PointInQuad(ray.O + ray.D * t, p5,  p6, p7, p8)){
  	if(newTmin(tmin, t)){
		tmin = t;
		Nmin = N4;
	}
  
  }
 
  //face p2, p3, p7, p6
  t = computeDistance(p2, N5, ray);
  if(PointInQuad(ray.O + ray.D * t, p2, p3, p7, p6)){
   	if(newTmin(tmin, t)){
		tmin = t;
		Nmin = N5;
	}
 
  }

  //face p1, p2, p6, p5
  t = computeDistance(p1, N6, ray);
  if(PointInQuad(ray.O + ray.D * t, p1, p2, p6, p5)){
   	if(newTmin(tmin, t)){
		tmin = t;
		Nmin = N6;
	}
 
  }
  if(tmin <= 0){
	  return Hit::NO_HIT();
  }

  t = tmin;
  N = Nmin;
  return Hit(t,N);
}

bool  Box::newTmin(double Tmin, double T){
	if(T <= 0){
		return false;
	}
	if(Tmin <= 0){
		return true;
	}
	if(T < Tmin){
		return true;
	}
	return false;
		
}
bool Box::SameSide(Point p1,Point p2, Point a,Point b){
   Point  cp1 = (b-a).cross(p1-a);
   Point  cp2 = (b-a).cross(p2-a);
    if( cp1.dot(cp2) >= 0 ){
	 return true;
    }else{
	 return false;
    }
}

bool Box::PointInQuad(Point p,Point a, Point b, Point c, Point d){
    if( SameSide(p,a, b,c) && SameSide(p,b, a,d) && SameSide(p,c, a,b) && SameSide(p, a, c, d)){
	return true;
    }else{
	return false;
    }
}

double Box::computeDistance(Point p, Vector N, const Ray &ray){
   double t;
   if(ray.D.dot(N) == 0){
     return 0;
   }
   t = (p.dot(N) - ray.O.dot(N))/ray.D.dot(N);
   return t;
}

void Box::InitBox() {
  //origine point quand centre = 0, 0, 0
  Point po1, po2, po3, po4, po5, po6, po7, po8; 

  //pointe de la pyramide
  po1.x = -dimension.x/2;
  po1.y = dimension.y/2;
  po1.z = dimension.z/2;

  po2.x = dimension.x/2;
  po2.y = dimension.y/2;
  po2.z = dimension.z/2;

  po3.x = dimension.x/2;
  po3.y = -dimension.y/2;
  po3.z = dimension.z/2;
  
  po4.x = -dimension.x/2;
  po4.y = -dimension.y/2;
  po4.z = dimension.z/2;

  po5.x = -dimension.x/2;
  po5.y = dimension.y/2;
  po5.z = -dimension.z/2;

  po6.x = dimension.x/2;
  po6.y = dimension.y/2;
  po6.z = -dimension.z/2;

  po7.x = dimension.x/2;
  po7.y = -dimension.y/2;
  po7.z = -dimension.z/2;

  po8.x = -dimension.x/2;
  po8.y = -dimension.y/2;
  po8.z = -dimension.z/2;
  
  
  p1 = rotatePoint(po1);
  p2 = rotatePoint(po2);
  p3 = rotatePoint(po3);
  p4 = rotatePoint(po4);
  p5 = rotatePoint(po5);
  p6 = rotatePoint(po6);
  p7 = rotatePoint(po7);
  p8 = rotatePoint(po8);

  p1 += position;
  p2 +=  position;
  p3 +=  position;
  p4 += position;
  p5 +=  position;
  p6 +=  position;
  p7 +=  position;
  p8 +=  position;



  //	Box faces order:
  //
  //    5 --------- 6
  //    / |       /|
  //   /  |      / |
  // 1 ---|----- 2 | 
  //   | 8/     |  /7
  //   | /      | /
  //   |/       |/
  //  4 -------- 3



  //Normal of p1, p2, p3
  N1 = computeFaceNormal(p1, p2, p3) ;
  N1.normalize();
  //Normal of p8, p3, p4
  N2 = computeFaceNormal(p8, p3, p4) ;
  N2.normalize();
  //Normal of p1, p4, p5
  N3 = computeFaceNormal(p1, p4, p5) ;
  N3.normalize();
  //Normal of p6, p7, p5
  N4 = computeFaceNormal(p6, p7, p5) ;
  N4.normalize();
  //Normal of p6, p2, p3
  N5 = computeFaceNormal(p6, p2, p3) ;
  N5.normalize();
  //Normal of p1, p5, p2
  N6 = computeFaceNormal(p1, p5, p2) ;
  N6.normalize();

}

Vector Box::computeFaceNormal(Point p1,Point p2, Point p3){
  Vector A = p2 - p1;
  Vector B = p3 - p2;
  Vector N = Triple(A.y*B.z-A.z*B.y, A.z*B.x-A.x*B.z, A.x*B.y-A.y*B.x) ;
  return N;

}
Point Box::rotatePoint(Point p){
   Point res = p;
   res.x = res.x;
   res.y =  res.y * cos(rotation.x) - res.z * sin(rotation.x);
   res.z = res.y * sin(rotation.x) + res.z * cos(rotation.x);

   res.x = res.x * cos(rotation.y) + res.z * sin(rotation.y);
   res.y =  res.y;
   res.z =  -res.x * sin(rotation.y) + res.z * cos(rotation.y);
  
   res.x = res.x * cos(rotation.z) - res.y * sin(rotation.z);
   res.y =  res.x * sin(rotation.z) + res.y * cos(rotation.z);
   res.z =  res.z;
   return res;
}
double Box::near(Triple eye)
{
   double d = 0;
   return d;
}

double Box::far(Triple eye)
{
   double d = 0;
   return d;
}

Color Box::getColor(Point hit, Point normal)
{
  return material->color;
}
