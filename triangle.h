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

    virtual Hit intersect(const Ray &ray) override;

    virtual double near(Triple eye) override;

    virtual double far(Triple eye) override;

    virtual bool SameSide(Point p1,Point p2, Point a,Point b);
    virtual bool PointInTriangle(Point p,Point a, Point b, Point c);
    virtual double computeDistance(Point p, Vector N, const Ray &ray);
    Point p1, p2, p3;
    Vector N;

  virtual Color mapping(Image *texture, Point hit) override  {
    return Color(0, 0, 0);
  }
  virtual Color getColor(Point hit) override;
  virtual Color UVMapping(Point hit) override {

    Triple uvw = barycentricCenter(p1, p2, hit);

    Triple uv= Triple(1, 0, 0) * uvw.x + Triple(0, 1, 0) * uvw.y + Triple(0, 0, 1) * uvw.z;

    return Color(uv.x, 0, uv.z);
  };

  Triple barycentricCenter(Point p1, Point p2, Point hit) {
    double triAreaU = ((p1 - hit).cross(p2 - hit)).length() / 2;
    double triAreaV = ((p3 - hit).cross(p2 - hit)).length() / 2;
    double triAreaW = 1 - triAreaU - triAreaV;

    return Triple(triAreaU, triAreaV, triAreaW);
  }
};


#endif /* end of include guard: TRIANGLE_H_115209AE */
