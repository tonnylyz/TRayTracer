//
// Created by Tonny on 4/6/2018.
//

#ifndef RAYTRACER_GEOMETRY_H
#define RAYTRACER_GEOMETRY_H

#include <glm/glm.hpp>

class intersection;
class ray;

class geometry {
public:
    glm::vec3 position;

    glm::vec3 ambient_color;
    glm::vec3 diffuse_color;
    glm::vec3 specular_color;
    glm::vec3 emission_color;
    float shininess;

    glm::mat4 transform;

    geometry(const glm::vec3 &p);

    virtual intersection intersect(ray *r) = 0;

};


#endif //RAYTRACER_GEOMETRY_H
