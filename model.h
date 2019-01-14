#ifndef MODEL_H
#define MODEL_H

#include "object.h"
#include "glm.h"
#include "triangle.h"

class Model : public Object {

  GLMmodel* model;
  std::string modelPath;
  std::string modelMaterial;
  std::vector<float> vertices;
  std::vector<float> normals;
  std::vector<float> texcoords;
  std::vector<float> facetnorms;
  std::vector<Triangle> triangles;
  std::vector<Material> materials;

public:
  Model(Triple position, std::string modelP, std::string materialP)
    : modelPath(modelP), modelMaterial(materialP) {
    model = glmReadOBJ(modelP.c_str());

    position.x = model->position[0];
    position.y = model->position[1];
    position.z = model->position[2];

    // for (std::size_t i=0; i < model->numvertices; ++i) {
    //   vertices.emplace_back(model->vertices[i]);
    // }

    // for (std::size_t i = 0; i < model->numnormals; ++i) {
    //   normals.emplace_back(model->normals[i]);
    // }

    // for (std::size_t i = 0; i < model->numtexcoords; ++i) {
    //   texcoords.emplace_back(model->texcoords[i]);
    // }

    // for (std::size_t i = 0; i < model->numfacetnorms; ++i) {
    //   facetnorms.emplace_back(model->facetnorms[i]);
    // }

    for (std::size_t i = 0; i < model->numtriangles; ++i) {
      Point p1{model->vertices[model->triangles->vindices[0]],
               model->vertices[model->triangles->vindices[0]+1],
               model->vertices[model->triangles->vindices[0]+2]};

      Point p2{model->vertices[model->triangles->vindices[1]],
               model->vertices[model->triangles->vindices[1] + 1],
               model->vertices[model->triangles->vindices[1] + 2]};

      Point p3{model->vertices[model->triangles->vindices[2]],
               model->vertices[model->triangles->vindices[2] + 1],
               model->vertices[model->triangles->vindices[2] + 2]};
      triangles.emplace_back(p1, p2, p3);
      triangles.back().InitTriangle();
    }
    /*
    for (std::size_t i = 0; i < model->numvertices; ++i) {
      materials.emplace_back(model->materials[i]);
    }
    */
  }

  virtual Hit intersect(const Ray &ray) {
    Hit min_hit(std::numeric_limits<double>::infinity(), Vector());
    Triangle* triangle = nullptr;
    for (auto &tri : triangles) {
      Hit hit = tri.intersect(ray);
      if (hit.t < min_hit.t) {
        min_hit = hit;
        triangle = &tri;
      }
    }

    if (!triangle) return Hit::NO_HIT();
    else return min_hit;
  }

  virtual double near(Triple eye) {
    return 0;
  }

  virtual double far(Triple eye) {
    return 0;
  }

  virtual Color getColor(Point hit) {
    for (auto& tri : triangles) {
      if (tri.PointInTriangle(hit, tri.p1, tri.p2, tri.p3)) {
        return tri.getColor(hit);
      }
    }
    return Color(0, 0, 0);
  }

  virtual Color UVMapping(Point hit) {
    return Color(0, 0, 0);
  }

  virtual Color mapping(Image *texture, Point hit) {
    return Color(0, 0, 0);
  }
};

#endif
