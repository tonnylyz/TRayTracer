//
// Created by Tonny on 4/6/2018.
//

#ifndef RAYTRACER_DIRECTIONAL_LIGHT_H
#define RAYTRACER_DIRECTIONAL_LIGHT_H

#include "light.h"

class directional_light : public light {
public:
    glm::vec3 direction;

    directional_light(const glm::vec3 &p, const glm::vec3 &c) : light(p, c) {
        direction = glm::normalize(p);
    }

    glm::vec3 lighting(const glm::vec3 &position, const glm::vec3 &normal, geometry *g, ray *r) override;

};


#endif //RAYTRACER_DIRECTIONAL_LIGHT_H
