//
// Created by Tonny on 4/7/2018.
//

#include "intersection.h"

#include <vector>
#include <iostream>
#include "ray.h"
#include "point_light.h"
#include "directional_light.h"
#include "math_util.h"

extern std::vector<light *> lights;
extern std::vector<directional_light> directional_lights;
extern std::vector<point_light> point_lights;

void intersection::transform(const glm::mat4 &tf) {
    position = trt::transform_vector(tf, position);
    normal = glm::vec3(glm::transpose(glm::inverse(tf)) * glm::vec4(normal, 0));
}

glm::vec3 intersection::color() {
    glm::vec3 result = glm::vec3(0, 0, 0);
    result += g->ambient_color;
    result += g->emission_color;
    result += lighting_color();
    result += reflecting_color();
    return trt::limit_to(result, 1.0f);
}

glm::vec3 intersection::lighting_color() {
    glm::vec3 result = glm::vec3(0, 0, 0);
    /*for (light *l : lights) {
        result += l->lighting(position, normal, g, r);
    }*/
    for (point_light &l : point_lights) {
        result += l.lighting(position, normal, g, r);
    }
    for (directional_light &l : directional_lights) {
        result += l.lighting(position, normal, g, r);
    }
    return result;
}

extern int max_depth;

glm::vec3 intersection::reflecting_color() {
    if (r->depth > max_depth || g->specular_color == glm::vec3(0, 0, 0)) {
        return glm::vec3(0, 0, 0);
    }

    glm::vec3 direction = r->direction - 2.0f * glm::normalize(normal) * dot(glm::normalize(r->direction), glm::normalize(normal));
    ray reflection = ray(trt::increase_little(position, normal), direction, r->depth + 1);
    return g->specular_color * reflection.color();
}

intersection::intersection(ray *r, geometry *g) {
    this->r = r;
    this->g = g;
    hit = false;
}

intersection::intersection() {
    r = nullptr;
    g = nullptr;
    hit = false;
}
