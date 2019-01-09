#ifndef BOX_H_115209AE
#define BOX_H_115209AE

#include "object.h"
//Box
class Box : public Object
{
public: 
    Box(Point position,Point dimension, Point rotation) : position(position), dimension(dimension), rotation(rotation) { }
    
    virtual void InitBox();
   
    virtual Vector computeFaceNormal(Point p1,Point p2, Point p3);

    virtual Point rotatePoint(Point p);

    virtual Hit intersect(const Ray &ray);

    virtual double near(Triple eye);

    virtual double far(Triple eye);

    virtual bool SameSide(Point p1,Point p2, Point a,Point b);
    virtual bool PointInQuad(Point p,Point a, Point b, Point c, Point d);
    virtual double computeDistance(Point p, Vector N, const Ray &ray);
    virtual bool newTmin(double Tmin, double T);
    const Triple position;
    const Triple dimension;//Larg*Haut*Prof
    const Triple rotation;
    Point p1, p2, p3, p4, p5, p6, p7, p8;
    Vector N1, N2, N3, N4, N5, N6;

  virtual Color mapping(Image *texture, Point hit) override  {
    return Color(0, 0, 0);
  }
  virtual Color getColor(Point hit, Point normal) override;
  virtual Color UVMapping(Point hit) override {
    return Color(0, 0, 0);
  };
};


#endif /* end of include guard: BOX_H_115209AE */
