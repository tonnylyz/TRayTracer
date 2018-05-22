//
// Created by Tonny on 4/7/2018.
//

#ifndef RAYTRACER_INTERSECTION_H
#define RAYTRACER_INTERSECTION_H

#include <glm/glm.hpp>

class ray;
class geometry;
class point_light;
class directional_light;

class intersection {
private:
public:

    ray *r;
    geometry *g;

    bool hit;
    glm::vec3 position;
    glm::vec3 normal;

    intersection();

    intersection(ray *r, geometry *g);

    void transform(const glm::mat4 &tf);

    glm::vec3 color();

    glm::vec3 lighting_color();

    glm::vec3 reflecting_color();

};


#endif //RAYTRACER_INTERSECTION_H
