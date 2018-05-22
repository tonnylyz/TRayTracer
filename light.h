//
// Created by Tonny on 4/6/2018.
//

#ifndef RAYTRACER_LIGHT_H
#define RAYTRACER_LIGHT_H

#include <glm/glm.hpp>

class geometry;
class ray;

class light {

public:
    glm::vec3 position;
    glm::vec3 color;

    light(const glm::vec3 &p, const glm::vec3 &c);

    virtual glm::vec3 lighting(const glm::vec3 &position, const glm::vec3 &normal, geometry *g, ray *r) = 0;
};


#endif //RAYTRACER_LIGHT_H
