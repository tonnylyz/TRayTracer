//
// Created by Tonny on 4/6/2018.
//

#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H


#include <glm/glm.hpp>
#include "transform.h"
#include "ray.h"

class camera {
public:

    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;
    float fov;


    camera(const glm::vec3 &p, const glm::vec3 &t, const glm::vec3 &u, float f);

    camera();

    void render(uint8_t *result) const ;

    static camera main_camera;
};


#endif //RAYTRACER_CAMERA_H
