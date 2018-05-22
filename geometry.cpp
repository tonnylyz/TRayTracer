//
// Created by Tonny on 4/7/2018.
//
#include <iostream>
#include "geometry.h"
#include "intersection.h"
#include "sphere.h"
#include "ray.h"
#include "triangle.h"
#include "math_util.h"

geometry::geometry(const glm::vec3 &p) {
    position = p;
}

sphere::sphere(const glm::vec3 &p, float r) : geometry(p) {
    radius = r;
}

intersection sphere::intersect(ray *r) {
    intersection result(r, this);

    glm::mat4 itf = glm::inverse(transform);
    glm::vec4 pt = itf * glm::vec4(r->origin, 1);
    glm::vec3 p0 = glm::vec3(pt) / pt.w;
    glm::vec3 p1 = glm::normalize(glm::vec3(itf * glm::vec4(r->direction, 0)));

    float a = trt::dot(p1, p1);
    float b = 2.0f * trt::dot(p1, p0 - position);
    float c = trt::dot(p0 - position, p0 - position) - radius * radius;

    float t1 = (-b + sqrt(b * b - 4.0f * a * c)) / (2.0f * a);
    float t2 = (-b - sqrt(b * b - 4.0f * a * c)) / (2.0f * a);

    float t;

    if (t1 > 0 && t2 > 0) {
        result.hit = true;
        t = glm::min(t1, t2);
    } else if (t1 * t2 < 0) {
        result.hit = true;
        t = glm::max(t1, t2);
    } else {
        return result;
    }

    result.position = p0 + t * p1;
    result.normal = glm::normalize(p0 + t * p1 - position);
    result.transform(transform);

    return result;
}


triangle::triangle(const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3) : geometry(glm::vec3(0, 0, 0)) {
    u = v1;
    v = v2;
    w = v3;
    normal = glm::normalize(-glm::cross(v2 - v1, v3 - v1));
}

intersection triangle::intersect(ray *r) {
    intersection result(r, this);

    glm::mat4 itf = glm::inverse(transform);
    glm::vec3 origin = trt::transform_vector(itf, r->origin);
    glm::vec3 direction = glm::vec3(itf * glm::vec4(r->direction, 0));

    if (trt::length(normal) == 0.0f) {
        return result;
    }

    normal = glm::normalize(normal);

    float t = (trt::dot(u, normal) - trt::dot(origin, normal)) / trt::dot(direction, normal);
    glm::vec3 hit_position = origin + t * direction;
    glm::vec3 lbd = glm::vec3(0, 0, 0);
    glm::vec3 t0 = v - u, t1 = w - u, t2 = hit_position - u;
    float denom = trt::dot(t0, t0) * trt::dot(t1, t1) - trt::dot(t0, t1) * trt::dot(t0, t1);
    lbd.x = (trt::dot(t1, t1) * trt::dot(t2, t0) - trt::dot(t0, t1) * trt::dot(t2, t1)) / denom;
    lbd.y = (trt::dot(t0, t0) * trt::dot(t2, t1) - trt::dot(t0, t1) * trt::dot(t2, t0)) / denom;
    lbd.z = 1.0f - lbd.x - lbd.y;

    if (t > 0
        && lbd.x >= 0 && lbd.x <= 1
        && lbd.y >= 0 && lbd.y <= 1
        && lbd.z >= 0 && lbd.z <= 1) {
        result.hit = true;
        result.position = hit_position;
        result.normal = -normal;
        result.transform(transform);
    }
    return result;
}
