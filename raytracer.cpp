//
//  Framework for a raytracer
//  File: raytracer.cpp
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Author: Maarten Everts
//
//  This framework is inspired by and uses code of the raytracer framework of
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html
//

#include "raytracer.h"
#include "object.h"
#include "sphere.h"
#include "plane.h"
#include "triangle.h"
#include "box.h"
#include "material.h"
#include "light.h"
#include "camera.h"
#include "image.h"
#include "yaml/yaml.h"
#include <ctype.h>
#include <fstream>
#include <assert.h>

// Functions to ease reading from YAML input
void operator >> (const YAML::Node& node, Triple& t);
Triple parseTriple(const YAML::Node& node);

void operator >> (const YAML::Node& node, Triple& t)
{
  if(node.size()==3){
    node[0] >> t.x;
    node[1] >> t.y;
    node[2] >> t.z;
  }
  else if(node.size()==2){
    node[0] >> t.x;
    node[1] >> t.y;
    node[1] >> t.z;
  }
}

Triple parseTriple(const YAML::Node& node)
{
  Triple t;
  node[0] >> t.x;
  node[1] >> t.y;
  node[2] >> t.z;
  return t;
}

Material* Raytracer::parseMaterial(const YAML::Node& node)
{

  Material *m = new Material();
  m->color = Color(0.0, 0.0, 0.0);

  if(node.FindValue("color")){
    node["color"] >> m->color;
  }

  if(node.FindValue("texture")) {
    std::string texSrc = node["texture"];
    if (texSrc == "UV") {
      m->showUV = true;
    } else {
      m->showUV = false;
      if (imageHandler.find(texSrc) == imageHandler.end()) {
        imageHandler[texSrc] = new Image(texSrc.c_str());
        if (imageHandler[texSrc]->width() == 0) {
          std::cout << "!!! Impossible de lire l'image"
                    << texSrc << "\n";
        } else {
          std::cout << "Importation de : " << texSrc << "\n";
        }
      }
      m->texture = imageHandler[texSrc];
    }
  } else {
    m->texture = nullptr;
  }

  if (node.FindValue("bump")) {
    std::string texSrc = node["bump"];
    if (imageHandler.find(texSrc) == imageHandler.end()) {
      imageHandler[texSrc] = new Image(texSrc.c_str());
      if (imageHandler[texSrc]->width() == 0) {
        std::cout << "!!! Impossible de lire l'image" << texSrc << "\n";
      } else {
        std::cout << "Importation de : " << texSrc << "\n";
      }
    }

    m->bump = imageHandler[texSrc];
  }

  node["ka"] >> m->ka;
  node["kd"] >> m->kd;
  node["ks"] >> m->ks;
  node["n"] >> m->n;
  return m;
}

Object* Raytracer::parseObject(const YAML::Node& node)
{
  Object *returnObject = NULL;
  std::string objectType;
  node["type"] >> objectType;

  if (objectType == "sphere") {
    Point pos;
    node["position"] >> pos;
    double r;
    double angle = 0;
    Triple vec = Triple(0.0, 0.0, 1.0);
    if(node.FindValue("angle")){
      node["angle"] >> angle;
      node["radius"][0] >> r;
      node["radius"][1] >> vec;
    }else{
      node["radius"] >> r;
    }
    Sphere *sphere = new Sphere(pos,r,angle,vec);
    returnObject = sphere;
  } else if (objectType == "plane") {
    Point normal;
    node["normal"] >> normal;
    double to;
    node["distance"] >> to;
    Plane *plane = new Plane(normal,to);
    returnObject = plane;
  }
    else if (objectType == "triangle") {
        Point p1;
        node["p1"] >> p1;
        Point p2;
        node["p2"] >> p2;
        Point p3;
        node["p3"] >> p3;
        Triangle *triangle = new Triangle(p1,p2,p3);
	triangle->InitTriangle();
        returnObject = triangle;
    }
    else if (objectType == "box") {
        Point position;
        node["position"] >> position;
        Point dimension;
        node["dimension"] >> dimension;
        Point rotation;
        node["rotation"] >> rotation;
        Box *box = new Box(position,dimension,rotation);
	box->InitBox();
        returnObject = box;
    }

  if (returnObject) {
    // read the material and attach to object
    returnObject->material = parseMaterial(node["material"]);
  }

  return returnObject;
}

Camera* Raytracer::parseCamera(const YAML::Node& node)
{
  Point eye;
  node["eye"] >> eye;
  Point center;
  node["center"] >> center;
  Point up;
  node["up"] >> up;
  Point viewSize;
  node["viewSize"] >> viewSize;
  Camera *returnCamera = new Camera(eye, center, up, viewSize);
  return returnCamera;
}

Light* Raytracer::parseLight(const YAML::Node& node)
{
  Point position;
  node["position"] >> position;
  Color color;
  node["color"] >> color;
  return new Light(position,color);
}


std::string Raytracer::parseRendering(const YAML::Node& node)
{
  std::string renderMode;
  node >> renderMode;
  return renderMode;
}

double Raytracer::parseRecDepth(const YAML::Node& node)
{
  double recDepth;
  node >> recDepth;
  return recDepth;
}

/*
 * Read a scene from file
 */

bool Raytracer::readScene(const std::string& inputFilename)
{
  // Initialize a new scene
  scene = new Scene();

  // Open file stream for reading and have the YAML module parse it
  std::ifstream fin(inputFilename.c_str());
  if (!fin) {
    cerr << "Error: unable to open " << inputFilename << " for reading." << endl;;
    return false;
  }
  try {
    YAML::Parser parser(fin);
    if (parser) {
      YAML::Node doc;
      parser.GetNextDocument(doc);

      if (doc.FindValue("GoochParameters")) {
            scene->setGoochParameters(
                doc["GoochParameters"]["alpha"], doc["GoochParameters"]["beta"],
                doc["GoochParameters"]["b"], doc["GoochParameters"]["y"]);
      }

      if (doc.FindValue("SuperSampling")) {
        scene->setSuperSampling(doc["SuperSampling"]["factor"]);
      }

      if(doc.FindValue("MaxRecursionDepth")){
        // Read and set recursion depth's value
        scene->setRecDepth(parseRecDepth(doc["MaxRecursionDepth"]));
      }

      if(doc.FindValue("RenderMode")){
        // Read and set scene Rendering to RenderMode directive's value
        scene->setRendering(parseRendering(doc["RenderMode"]));
      } else{
        // If no renderingMode directive found then set rendering to Phong by default
        const char* phong = "phong";
        std::string str(phong);
        scene->setRendering(str);
      }

      if (doc.FindValue("Shadows")) {
        scene->renderShadows(doc["Shadows"]);
      } else {
        scene->renderShadows(false);
      }

      if(doc.FindValue("Camera")){
        scene->setCamera(parseCamera(doc["Camera"]));
        scene->withCam = true;

      }

      if(doc.FindValue("Eye")){
        scene->setEye(parseTriple(doc["Eye"]));
        scene->withEye = true;
      }

      // Read and parse the scene objects
      const YAML::Node& sceneObjects = doc["Objects"];
      if (sceneObjects.GetType() != YAML::CT_SEQUENCE) {
        cerr << "Error: expected a sequence of objects." << endl;
        return false;
      }
      for(YAML::Iterator it=sceneObjects.begin();it!=sceneObjects.end();++it) {
        Object *obj = parseObject(*it);
        // Only add object if it is recognized
        if (obj) {
          scene->addObject(obj);
        } else {
          cerr << "Warning: found object of unknown type, ignored." << endl;
        }
      }

      // Read and parse light definitions
      const YAML::Node& sceneLights = doc["Lights"];
      if (sceneObjects.GetType() != YAML::CT_SEQUENCE) {
        cerr << "Error: expected a sequence of lights." << endl;
        return false;
      }
      for(YAML::Iterator it=sceneLights.begin();it!=sceneLights.end();++it) {
        scene->addLight(parseLight(*it));
      }
    }
    if (parser) {
      cerr << "Warning: unexpected YAML document, ignored." << endl;
    }
  } catch(YAML::ParserException& e) {
    std::cerr << "Error at line " << e.mark.line + 1 << ", col " << e.mark.column + 1 << ": " << e.msg << std::endl;
    return false;
  }

  cout << "YAML parsing results: " << scene->getNumObjects() << " objects read." << endl;
  return true;
}

void Raytracer::renderToFile(const std::string& outputFilename)
{
  if(scene->withCam){
    Image img(static_cast<int>(scene->getWidth()),static_cast<int>(scene->getHeight()));
    cout << "Tracing..." << endl;
    scene->render(img);
    cout << "Writing image to " << outputFilename << "..." << endl;
    img.write_png(outputFilename.c_str());
    cout << "Done." << endl;
  }else{
    Image img(400,400);
    cout << "Tracing..." << endl;
    scene->render(img);
    cout << "Writing image to " << outputFilename << "..." << endl;
    img.write_png(outputFilename.c_str());
    cout << "Done." << endl;
  }
}
