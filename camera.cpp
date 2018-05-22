//
// Created by Tonny on 4/7/2018.
//
#include <iostream>
#include "camera.h"
#include "string_util.h"
#include <thread>

camera::camera(const glm::vec3 &p, const glm::vec3 &t, const glm::vec3 &u, float f) {
    position = p;
    target = t;
    up = transform::up_vector(u, target - position);
    fov = f;
}

camera::camera() {
    position = glm::vec3();
    target = glm::vec3();
    up = glm::vec3();
    fov = 0;
}

extern unsigned int image_width;
extern unsigned int image_height;

void camera::render(uint8_t *result) const {

    int total_pixel = image_width * image_height;

    glm::vec3 w = glm::normalize(position - target);
    glm::vec3 u = glm::normalize(glm::cross(w, up));
    glm::vec3 v = glm::cross(w, u);


    std::vector<std::thread> workers;

#define THREAD_NUMBER 4

    workers.push_back(std::thread([&]() {

        for (unsigned int j = 0; j < image_height / THREAD_NUMBER; j++) {
            for (unsigned int i = 0; i < image_width; i++) {

                float alpha = -tan(glm::radians(fov / 2)) * image_width / image_height * (i + 0.5f - image_width / 2.0f) /
                              (image_width / 2.0f);
                float beta = -tan(glm::radians(fov / 2)) * (j + 0.5f - image_height / 2.0f) / (image_height / 2.0f);

                glm::vec3 direction = glm::normalize(alpha * u + beta * v - w);

                ray r(position, direction);
                glm::vec3 color = r.color();

                unsigned int pixel = j * image_width + i;
                result[3 * pixel + 0] = (uint8_t) (255 * color.z);
                result[3 * pixel + 1] = (uint8_t) (255 * color.y);
                result[3 * pixel + 2] = (uint8_t) (255 * color.x);

            }
        }

    }));
    workers.push_back(std::thread([&]() {

        for (unsigned int j = image_height / THREAD_NUMBER; j < 2 * image_height / THREAD_NUMBER; j++) {
            for (unsigned int i = 0; i < image_width; i++) {

                float alpha = -tan(glm::radians(fov / 2)) * image_width / image_height * (i + 0.5f - image_width / 2.0f) /
                              (image_width / 2.0f);
                float beta = -tan(glm::radians(fov / 2)) * (j + 0.5f - image_height / 2.0f) / (image_height / 2.0f);

                glm::vec3 direction = glm::normalize(alpha * u + beta * v - w);

                ray r(position, direction);
                glm::vec3 color = r.color();

                unsigned int pixel = j * image_width + i;
                result[3 * pixel + 0] = (uint8_t) (255 * color.z);
                result[3 * pixel + 1] = (uint8_t) (255 * color.y);
                result[3 * pixel + 2] = (uint8_t) (255 * color.x);

            }
        }

    }));
    workers.push_back(std::thread([&]() {

        for (unsigned int j = 2 * image_height / THREAD_NUMBER; j < 3 * image_height / THREAD_NUMBER; j++) {
            for (unsigned int i = 0; i < image_width; i++) {

                float alpha = -tan(glm::radians(fov / 2)) * image_width / image_height * (i + 0.5f - image_width / 2.0f) /
                              (image_width / 2.0f);
                float beta = -tan(glm::radians(fov / 2)) * (j + 0.5f - image_height / 2.0f) / (image_height / 2.0f);

                glm::vec3 direction = glm::normalize(alpha * u + beta * v - w);

                ray r(position, direction);
                glm::vec3 color = r.color();

                unsigned int pixel = j * image_width + i;
                result[3 * pixel + 0] = (uint8_t) (255 * color.z);
                result[3 * pixel + 1] = (uint8_t) (255 * color.y);
                result[3 * pixel + 2] = (uint8_t) (255 * color.x);

            }
        }

    }));
    workers.push_back(std::thread([&]() {

        for (unsigned int j = 3 * image_height / THREAD_NUMBER; j < 4 * image_height / THREAD_NUMBER; j++) {
            for (unsigned int i = 0; i < image_width; i++) {

                float alpha = -tan(glm::radians(fov / 2)) * image_width / image_height * (i + 0.5f - image_width / 2.0f) /
                              (image_width / 2.0f);
                float beta = -tan(glm::radians(fov / 2)) * (j + 0.5f - image_height / 2.0f) / (image_height / 2.0f);

                glm::vec3 direction = glm::normalize(alpha * u + beta * v - w);

                ray r(position, direction);
                glm::vec3 color = r.color();

                unsigned int pixel = j * image_width + i;
                result[3 * pixel + 0] = (uint8_t) (255 * color.z);
                result[3 * pixel + 1] = (uint8_t) (255 * color.y);
                result[3 * pixel + 2] = (uint8_t) (255 * color.x);

            }
        }

    }));

    std::for_each(workers.begin(), workers.end(), [](std::thread &t) {
        t.join();
    });
}
