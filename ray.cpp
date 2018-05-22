//
// Created by Tonny on 4/7/2018.
//

#include <iostream>
#include "ray.h"
#include "geometry.h"
#include "intersection.h"
#include "sphere.h"
#include "triangle.h"
#include "math_util.h"

extern int max_depth;
extern std::vector<geometry *> geometries;
extern std::vector<sphere> spheres;
extern std::vector<triangle> triangles;


ray::ray(const glm::vec3 &o, const glm::vec3 &d) {
    origin = o;
    direction = glm::normalize(d);
    depth = 0;
}

ray::ray(const glm::vec3 &o, const glm::vec3 &d, int depth) : ray(o, d) {
    this->depth = depth;
}

std::vector <intersection> ray::intersections() {
    std::vector<intersection> result;
    /*for (geometry *g : geometries) {
        intersection i = g->intersect(this);
        if (i.hit) {
            result.push_back(i);
        }
    }*/
    for (sphere &s_obj : spheres) {
        intersection i = s_obj.intersect(this);
        if (i.hit) {
            result.push_back(i);
        }
    }
    for (triangle &t_obj : triangles) {
        intersection i = t_obj.intersect(this);
        if (i.hit) {
            result.push_back(i);
        }
    }
    return result;
}

glm::vec3 ray::color() {
    glm::vec3 result(0, 0, 0);
    std::vector<intersection> its = intersections();
    if (its.empty()) {
        return result;
    }
    float min_distance = 0;
    intersection *min_intersection = nullptr;
    for (intersection &i : its) {
        float distance = trt::length(i.position - origin);
        if (min_intersection == nullptr || distance < min_distance) {
            min_distance = distance;
            min_intersection = &i;
        }
    }
    if (min_intersection == nullptr) {
        return result;
    }
    result = min_intersection->color();
    return result;
}
