//
// Created by Tonny on 4/7/2018.
//

#ifndef RAYTRACER_MATH_UTIL_H
#define RAYTRACER_MATH_UTIL_H

#include <glm/glm.hpp>

namespace trt {
    static inline float dot(const glm::vec3 &a, const glm::vec3 &b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    static inline float length(const glm::vec3 &a) {
        return sqrt(dot(a, a));
    }

    static inline float max(float a, float b) {
        return a > b ? a : b;
    }

    static inline glm::vec3 increase_little(const glm::vec3 &pos, const glm::vec3 &direction) {
        return pos + 0.001f * glm::normalize(direction);
    }

    static inline glm::vec3 limit_to(const glm::vec3 &a, float limit) {
        return glm::vec3(
                a.x > limit ? limit : a.x,
                a.y > limit ? limit : a.y,
                a.z > limit ? limit : a.z
        );
    }

    static inline glm::vec3 transform_vector(const glm::mat4 &tf, const glm::vec3 &v) {
        glm::vec4 u = tf * glm::vec4(v, 1);
        return glm::vec3(u) / u.w;
    }
}

#endif //RAYTRACER_MATH_UTIL_H
