//
// Created by Tonny on 4/7/2018.
//
#include "transform.h"
#include <cmath>

glm::mat3 transform::rotate(const float degrees, const glm::vec3 &axis) {
    const auto identity_3 = glm::mat3(1, 0, 0, 0, 1, 0, 0, 0, 1);
    const auto axis_axis_t = glm::mat3(
            axis.x * axis.x, axis.x * axis.y, axis.x * axis.z,
            axis.x * axis.y, axis.y * axis.y, axis.y * axis.z,
            axis.x * axis.z, axis.y * axis.z, axis.z * axis.z);
    const auto a_star = glm::mat3(
            0, -axis.z, axis.y,
            axis.z, 0, -axis.x,
            -axis.y, axis.x, 0
    );
    const auto theta = -degrees * (pi / 180);
    auto result = identity_3 * cos(theta) + (1 - cos(theta)) * axis_axis_t + sin(theta) * a_star;
    return result;
}

void transform::left(float degrees, glm::vec3 &eye, glm::vec3 &up) {
    const auto result = rotate(degrees, up) * eye;
    eye = result;
}

void transform::up(float degrees, glm::vec3 &eye, glm::vec3 &up) {
    auto axis = glm::cross(eye, up);
    axis = glm::normalize(axis);
    const auto result = rotate(degrees, axis) * eye;
    eye = result;
    up = -glm::cross(eye, axis);
    up = glm::normalize(up);
}

glm::mat4 transform::lookAt(const glm::vec3 &eye, const glm::vec3 &center, const glm::vec3 &up) {
    auto w = glm::normalize(eye - center);
    auto u = glm::normalize(glm::cross(-w, up));
    auto v = glm::cross(w, u);
    auto result = glm::mat4(
            u.x, u.y, u.z, -glm::dot(u, eye),
            v.x, v.y, v.z, -glm::dot(v, eye),
            w.x, w.y, w.z, -glm::dot(w, eye),
            0, 0, 0, 1
    );
    return glm::transpose(result);
}

glm::mat4 transform::perspective(float fovy, float aspect, float zNear, float zFar) {
    float t = (float)tan(glm::radians(fovy) / 2.0);
    auto result = glm::mat4(
            1.0 / (aspect * t), 0, 0, 0,
            0, 1.0 / t, 0, 0,
            0, 0, -(zFar + zNear) / (zFar - zNear), -(2.0 * zFar * zNear) / (zFar - zNear),
            0, 0, -1.0, 0
    );
    return glm::transpose(result);
}

glm::mat4 transform::scale(const float &sx, const float &sy, const float &sz) {
    auto result = glm::mat4(1.0);
    result[0][0] = sx;
    result[1][1] = sy;
    result[2][2] = sz;
    return result;
}

glm::mat4 transform::translate(const float &tx, const float &ty, const float &tz) {
    auto result = glm::mat4(1.0);
    result[3][0] = tx;
    result[3][1] = ty;
    result[3][2] = tz;
    return result;
}

glm::vec3 transform::up_vector(const glm::vec3 &up, const glm::vec3 &zvec) {
    glm::vec3 x = glm::cross(up, zvec);
    glm::vec3 y = glm::cross(zvec, x);
    glm::vec3 ret = glm::normalize(y);
    return ret;
}
