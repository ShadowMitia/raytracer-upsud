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
#include "camera.h"

class Scene
{
private:
  std::vector<Object*> objects;
  std::vector<Light*> lights;
  Triple eye;
  Camera* camera;
  std::string rendering;
  double near = 0;
  double far = 0;

  int RecDepth = 0;
  bool shadows = false;
  int superSampling = 1;

  // gooch parameters
  double alpha = 0.25;
  double beta = 0.5;
  double b = 0.55;
  double y = 0.3;

public:
  Color trace(const Ray &ray, int recDepth);

  void render(Image &img);
  void renderEye(Image &img);
  void renderCam(Image &img);
  void addObject(Object *o);
  void addLight(Light *l);
  void setEye(Triple e);
  void setCamera(Camera *c);
  void setGoochParameters(double alpha_, double beta_, double b_, double y_) {
    alpha = alpha_;
    beta = beta_;
    b = b_;
    y = y_;
  }
  double getWidth(){return camera->viewSize.x;}
  double getHeight(){return camera->viewSize.y;};
  std::size_t getNumObjects() { return objects.size(); }
  std::size_t getNumLights() { return lights.size(); }
  void setRendering(std::string r);
  std::string getRendering();
  void computeNearFar();
  double getNear();
  double getFar();
  void setRecDepth(double r);
  double getRecDepth();
  void renderShadows(bool toggle) { shadows = toggle; }
  void setSuperSampling(int value)  { superSampling = value; }
  bool withEye = false;
  bool withCam = false;
};

#endif /* end of include guard: SCENE_H_KNBLQLP6 */
