//
//  Framework for a raytracer
//  File: raytracer.h
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

#ifndef RAYTRACER_H_6GQO67WK
#define RAYTRACER_H_6GQO67WK

#include <iostream>
#include <string>
#include <unordered_map>
#include "triple.h"
#include "light.h"
#include "scene.h"
#include "yaml/yaml.h"

class Raytracer {
private:
    Scene *scene;

    // Couple of private functions for parsing YAML nodes
    Material* parseMaterial(const YAML::Node& node);
    Object* parseObject(const YAML::Node& node);
    Object *parseOBJ(const YAML::Node &node);
    Light* parseLight(const YAML::Node& node);
    std::string parseRendering(const YAML::Node& node);
    double parseRecDepth(const YAML::Node& node);
    Camera* parseCamera(const YAML::Node& node);

  std::unordered_map<std::string, Image*> imageHandler;

public:
    Raytracer() { }

    bool readScene(const std::string& inputFilename);
    void renderToFile(const std::string& outputFilename);
};

#endif /* end of include guard: RAYTRACER_H_6GQO67WK */
