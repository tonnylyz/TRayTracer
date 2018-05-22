//
// Created by Tonny on 4/6/2018.
//

#ifndef RAYTRACER_TRIANGLE_H
#define RAYTRACER_TRIANGLE_H

#include "geometry.h"

class triangle : public geometry {
private:
    glm::vec3 u;
    glm::vec3 v;
    glm::vec3 w;

    glm::vec3 normal;

public:
    triangle(const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3);

    intersection intersect(ray *r) override;
};


#endif //RAYTRACER_TRIANGLE_H
