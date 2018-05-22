//
// Created by Tonny on 4/7/2018.
//

#ifndef RAYTRACER_RAY_H
#define RAYTRACER_RAY_H

#include <vector>
#include <glm/glm.hpp>

class intersection;

class ray {
public:
    glm::vec3 origin;
    glm::vec3 direction;
    int depth;

    ray(const glm::vec3 &o, const glm::vec3 &d);

    ray(const glm::vec3 &o, const glm::vec3 &d, int depth);

    std::vector<intersection> intersections();

    glm::vec3 color();
};


#endif //RAYTRACER_RAY_H
