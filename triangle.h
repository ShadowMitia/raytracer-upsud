#ifndef TRIANGLE_H_115209AE
#define TRIANGLE_H_115209AE

#include "object.h"
//Pyramide base rectangle
class Triangle : public Object
{
public: 
    Triangle(Point p1,Point p2, Point p3) : p1(p1), p2(p2), p3(p3) { }
    
    virtual void InitTriangle();
   
    virtual Vector computeFaceNormal(Point p1,Point p2, Point p3);

    virtual Hit intersect(const Ray &ray);

    virtual double near(Triple eye);

    virtual double far(Triple eye);

    virtual bool SameSide(Point p1,Point p2, Point a,Point b);
    virtual bool PointInTriangle(Point p,Point a, Point b, Point c);
    virtual double computeDistance(Point p, Vector N, const Ray &ray);
    Point p1, p2, p3;
    Vector N;

  virtual Color mapping(Image *texture, Point hit) override  {
    return Color(0, 0, 0);
  }
  virtual Color getColor(Point hit, Point normal) override;
  virtual Color UVMapping(Point hit) override {
    return Color(0, 0, 0);
  };
};


#endif /* end of include guard: TRIANGLE_H_115209AE */
