//
//  Framework for a raytracer
//  File: scene.h
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

#ifndef SCENE_H_KNBLQLP6
#define SCENE_H_KNBLQLP6

#include <vector>
#include "triple.h"
#include "light.h"
#include "object.h"
#include "image.h"

class Scene
{
private:
    std::vector<Object*> objects;
    std::vector<Light*> lights;
    Triple eye;
    std::string rendering;
    double near = 0;
    double far = 0;

  double RecDepth = 0;
  bool shadows = false;
public:
    Color trace(const Ray &ray, int recDepth);

    void render(Image &img);
    void addObject(Object *o);
    void addLight(Light *l);
    void setEye(Triple e);
    unsigned int getNumObjects() { return objects.size(); }
    unsigned int getNumLights() { return lights.size(); }
    void setRendering(std::string r);
    std::string getRendering();
    void computeNearFar();
    double getNear();
    double getFar();
    void setRecDepth(double r);
    double getRecDepth();
  void renderShadows(bool toggle) { shadows = toggle; }
};

#endif /* end of include guard: SCENE_H_KNBLQLP6 */
