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


void Scene::computeNearFar()
{
  if(objects.size() != 0){
    far = objects[0]->far(eye);
    near = objects[0]->near(eye);

    for (unsigned int i = 1; i < objects.size(); ++i) {
      if(objects[i]->far(eye)>far){
        far = objects[i]->far(eye);
      }
      if(objects[i]->near(eye)<near){
        near = objects[i]->near(eye);
      }
    }
    far = far * 1.3;
  }
}

double Scene::getNear()
{
  if (near == far ){
    computeNearFar();
  }
  return near;
}

double Scene::getFar()
{
  if (near == far ){
    computeNearFar();
  }
  return far;
}

/*Color Scene::normalsTrace(const Ray& ray) {
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

N += Vector(1.0, 1.0, 1.0);
N.normalize();
return N;
}*/

Color Scene::trace(const Ray &ray, int recDepth)
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

  /*
   *	Phong illumination
   */

  //If the rendering mode is "phong"
  if(rendering == "phong"){
    Color c;
    //Reflected intensities
    double Id = 0;
    double Ia = 0;
    Color cd = Color(0.0, 0.0, 0.0);
    Color ca = Color(0.0, 0.0, 0.0);
    //Specular color
    Color cs = Color(0.0, 0.0, 0.0);
    //Reflection color
    Color cr = Color(0.0, 0.0, 0.0);


    for( size_t i = 0; i < lights.size() ; ++i){
      //Ambiant
      Ia = material->ka;
      ca += lights[i]->color * Ia;
      N.normalize();

      //Hit to light vector
      Vector L = lights[i]->position - hit;
      L.normalize();
      //Diffuse
      double diff = material->kd * L.dot(N);

      bool hasHit = false;
      if (shadows) {
        Ray lightRay(hit, L);

        for (size_t j = 0; j < objects.size(); j++) {
          Hit hit(objects[j]->intersect(lightRay));
          if (!hit.no_hit) {
            hasHit = true;
          }
        }
      }

      //If cosinus is negative (in the shadow area) then the diff isn't taken into account
      if(!hasHit && diff > 0){
        Id += diff;
        cd += (lights[i]->color) *  Id;
        //Vector of reflected light
        Vector R = (2 * N.dot(L) * N) - L;
        R.normalize();
        //cosRV = cosinus of angle between R and V
        double cosRV = R.dot(V);

        Vector H = L+V;
        H.normalize();

        //If cosinus is negative (at the opposite of the view) then the spec isn't taken into account
        if(cosRV > 0){
          //Specular
          double spec = material->ks * pow(H.dot(N), material->n);
          cs += lights[i]->color * spec;
        }
      }
    }

    if(recDepth > 0) {
      //Vector of reflected light
      Vector RR =  (2.0 * N.dot(ray.D) * N) - ray.D;
      RR.normalize();
      Ray rray(hit, -RR);
      cr = trace(rray, recDepth-1);
    }

    //final color
    color = (ca + cd) * color + (cs + cr) * material->ks;
  }

  /*
   *	z-buffer
   */

  //If the rendering mode is "zbuffer"
  if(rendering == "zbuffer"){
    //distance on z axis between the eye and the intersection point
    double z = sqrt((min_hit.t * min_hit.t) - (eye.y - hit.y) * (eye.y - hit.y) - (eye.x - hit.x) * (eye.x - hit.x));
    //The range of depth values between -1 and 1
    double zp = ( ( getFar() + getNear() ) / ( getFar() - getNear() ) )
      +
      ( (1/z) * ( ( -2 * getFar() * getNear() ) / ( getFar() - getNear() ) ) );
    //The gray level representing the depth value
    double grayLev = 1 -( (zp + 1)/2);
    color.set(grayLev);
  }

  /*
   *	normal
   */

  //If the rendering mode is "normal"
  if(rendering == "normal") {
    N += Vector(1.0, 1.0, 1.0);
    N /= 2;
    color = N;
  }

  return color;
}

void Scene::render(Image &img)
{
    int w = img.width();
    int h = img.height();
    for (int y = 0; y < h; y++) {
      for (int x = 0; x < w; x++) {
        Color averageColor;

        for (int i = 0; i < superSampling; i++) {
          for (int j = 0; j < superSampling; j++) {

            float sample = (superSampling+1)/2;
            float sampling = 1.0 / (2.0 * sample);

            float subPixelX = x + sampling + i * 2.0 * sampling;
            float subPixelY = h - 1.0 - y + sampling + j * 2.0 * sampling;

            //cout << subPixelX << " " << subPixelY << "\n";

            Point pixel(subPixelX, subPixelY, 0);
            Ray ray(eye, (pixel-eye).normalized());
            Color col = trace(ray, getRecDepth());
            //col.clamp();
            averageColor += col;
          }
        }

        averageColor /= (superSampling) * (superSampling);
        averageColor.clamp();
        img(x,y) = averageColor;
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

void Scene::setRendering(std::string r){
  rendering = r;
}

std::string Scene::getRendering(){
  return rendering;
}

void Scene::setRecDepth(double r){
  RecDepth = r;
}

double Scene::getRecDepth(){
  return RecDepth;
}
