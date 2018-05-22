//
// Created by Tonny on 4/6/2018.
//

#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H

#include "geometry.h"

class sphere : public geometry {
private:
    float radius;
public:

    sphere(const glm::vec3 &p, float r);

    intersection intersect(ray *r) override;

};


#endif //RAYTRACER_SPHERE_H
