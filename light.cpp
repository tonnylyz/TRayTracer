//
// Created by Tonny on 4/7/2018.
//
#include <iostream>
#include "light.h"
#include "intersection.h"
#include "point_light.h"
#include "directional_light.h"
#include "math_util.h"

light::light(const glm::vec3 &p, const glm::vec3 &c) {
    position = p;
    color = c;
}

glm::vec3 point_light::lighting(const glm::vec3 &position, const glm::vec3 &normal, geometry *g, ray *r) {
    glm::vec3 direction = this->position - position;
    float distance = trt::length(direction);
    direction = glm::normalize(direction);
    ray light_ray(trt::increase_little(position, direction), direction);
    for (intersection &i : light_ray.intersections()) {
        if (trt::length(i.position - i.r->origin) < distance) {
            return glm::vec3(0, 0, 0); // shadow
        }
    }
    float attenuation = attenuation_const + attenuation_linear * distance +
                        attenuation_quadratic * distance * distance;
    glm::vec3 light_color = color / attenuation;
    glm::vec3 half_angle = glm::normalize(direction - r->direction);
    glm::vec3 result = light_color *
                       (g->diffuse_color * trt::max(trt::dot(glm::normalize(normal), direction), 0.0f)
                        + g->specular_color * pow(trt::max(trt::dot(glm::normalize(normal), half_angle), 0.0f), g->shininess));
    return result;
}


glm::vec3 directional_light::lighting(const glm::vec3 &position, const glm::vec3 &normal, geometry *g, ray *r) {
    ray light_ray(trt::increase_little(position, direction), direction);
    if (!light_ray.intersections().empty()) {
        return glm::vec3(0, 0, 0); // shadow
    }

    glm::vec3 half_angle = glm::normalize(direction - r->direction);
    glm::vec3 result = color *
           (g->diffuse_color * trt::max(trt::dot(glm::normalize(normal), light_ray.direction), 0.0f)
            + g->specular_color * pow(trt::max(trt::dot(glm::normalize(normal), half_angle), 0.0f), g->shininess));
    return result;
}
