//
// Created by Tonny on 4/6/2018.
//

#ifndef RAYTRACER_SPOT_LIGHT_H
#define RAYTRACER_SPOT_LIGHT_H

#include "light.h"
#include "geometry.h"
#include "ray.h"

extern float attenuation_const;
extern float attenuation_linear;
extern float attenuation_quadratic;

class point_light : public light {
public:

    point_light(const glm::vec3 &p, const glm::vec3 &c) : light(p, c) {

    }

    glm::vec3 lighting(const glm::vec3 &position, const glm::vec3 &normal, geometry *g, ray *r) override;

};


#endif //RAYTRACER_SPOT_LIGHT_H
