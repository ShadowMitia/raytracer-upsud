//
//  Framework for a raytracer
//  File: scene.cpp
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

#include "scene.h"
#include "material.h"

Color Scene::trace(const Ray &ray)
{
  // Find hit object and distance
  Hit min_hit(std::numeric_limits<double>::infinity(),Vector());
  Object *obj = NULL;
  for (unsigned int i = 0; i < objects.size(); ++i) {
    Hit hit(objects[i]->intersect(ray));
    if (hit.t<min_hit.t) {
      min_hit = hit;
      obj = objects[i];
    }
  }

  // No hit? Return background color.
  if (!obj) return Color(0.0, 0.0, 0.0);

  Material *material = obj->material;            //the hit objects material
  Point hit = ray.at(min_hit.t);                 //the hit point
  Vector N = min_hit.N;                          //the normal at hit point
  Vector V = -ray.D;                             //the view vector


  /****************************************************
   * This is where you should insert the color
   * calculation (Phong model).
   *
   * Given: material, hit, N, V, lights[]
   * Sought: color
   *
   * Hints: (see triple.h)
   *        Triple.dot(Vector) dot product
   *        Vector+Vector      vector sum
   *        Vector-Vector      vector difference
   *        Point-Point        yields vector
   *        Vector.normalize() normalizes vector, returns length
   *        double*Color        scales each color component (r,g,b)
   *        Color*Color        dito
   *        pow(a,b)           a to the power of b
   ****************************************************/

  Color color = material->color;                  // place holder

  //Reflected intensities
  double Id = 0;
  double Is = 0;
  double Ia = 0;

  //Ambiant
  double ia = 1;
  Ia = ia * material->ka;
  N.normalize();

  for( size_t i = 0; i < lights.size() ; ++i){
    //Hit to light vector
    Vector L = lights[i]->position - hit;
    L.normalize();
    //Diffuse
    double id = 1;
    double diff = id * material->kd * L.dot(N);
   
    //If cosinus is negative (in the shadow area) then the diff isn't taken into account
    if(diff > 0){
      Id += diff;
      //Vector of reflected light
      Vector R = (2 * N.dot(L) * N) - L;
      R.normalize();
      //Specular
      double is = 1;
      //scale = cosinus of angle between R and V
      double scale = R.dot(V);
      //If cosinus is negative (at the opposite of the view) then the spec isn't taken into account 
      if(scale > 0){
	      //alpha = shininess constant
	      double alpha = 20;
	      double spec = is * material->ks * pow(scale, alpha);
	      Is += spec;
      }
    }
  }

 

  //final color
  color = ((Ia + Id) * color + Is * Color(1.0, 1.0, 1.0));

  return color;
}

void Scene::render(Image &img)
{
  int w = img.width();
  int h = img.height();
  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      Point pixel(x+0.5, h-1-y+0.5, 0);
      Ray ray(eye, (pixel-eye).normalized());
      Color col = trace(ray);
      col.clamp();
      img(x,y) = col;
    }
  }
}

void Scene::addObject(Object *o)
{
  objects.push_back(o);
}

void Scene::addLight(Light *l)
{
  lights.push_back(l);
}

void Scene::setEye(Triple e)
{
  eye = e;
}
