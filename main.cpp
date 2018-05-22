#include <iostream>
#include <fstream>
#include <stack>
#include <vector>
#include <sstream>
#include <glm/glm.hpp>
#include <FreeImage.h>
#include "camera.h"
#include "directional_light.h"
#include "point_light.h"
#include "ray.h"
#include "intersection.h"
#include "sphere.h"
#include "string_util.h"
#include "transform.h"
#include "triangle.h"

unsigned int image_width;
unsigned int image_height;
int max_depth;
std::string output_filename;

float attenuation_const;
float attenuation_linear;
float attenuation_quadratic;

glm::vec3 ambient_color;
glm::vec3 diffuse_color;
glm::vec3 specular_color;
glm::vec3 emission_color;
float shininess;

std::vector<light *> lights;
std::vector<directional_light> directional_lights;
std::vector<point_light> point_lights;

std::vector<glm::vec3> vertices;

std::vector<geometry *> geometries;
std::vector<sphere> spheres;
std::vector<triangle> triangles;

camera camera::main_camera;

static inline void right_multiply(const glm::mat4 &M, std::stack<glm::mat4> &stack) {
    glm::mat4 &T = stack.top();
    T = T * M;
}

void read_scene_file(const std::string &path) {
    std::string str;
    std::ifstream in(path);
    if (!in.is_open()) {
        std::cerr << "Failed to open file `" << path << "`" << std::endl;
        return;
    }

    std::stack<glm::mat4> transformStack;
    transformStack.push(glm::mat4(1.0));

    for (std::string line; getline(in, line);) {
        trim(line);
        if (line[0] == '#') continue;
        if (line.empty()) continue;
        std::stringstream ss;
        ss << line;
        std::string cmd;
        ss >> cmd;

        if (cmd == "size") {
            ss >> image_width >> image_height;
        } else if (cmd == "maxdepth") {
            ss >> max_depth;
        } else if (cmd == "output") {
            ss >> output_filename;
        } else if (cmd == "camera") {
            float x, y, z, fov;
            ss >> x >> y >> z;
            glm::vec3 look_from(x, y, z);
            ss >> x >> y >> z;
            glm::vec3 look_at(x, y, z);
            ss >> x >> y >> z;
            glm::vec3 up(x, y, z);
            ss >> fov;
            camera::main_camera = camera(look_from, look_at, up, fov);
        } else
            // LIGHT
        if (cmd == "directional") {
            float x, y, z, r, g, b;
            ss >> x >> y >> z >> r >> g >> b;
            directional_lights.emplace_back(directional_light(glm::vec3(x, y, z), glm::vec3(r, g, b)));
            lights.push_back(&directional_lights.back());
        } else if (cmd == "point") {
            float x, y, z, r, g, b;
            ss >> x >> y >> z >> r >> g >> b;
            point_lights.emplace_back(point_light(glm::vec3(x, y, z), glm::vec3(r, g, b)));
            lights.push_back(&point_lights.back());
        } else
            // GEOMETRY
        if (cmd == "maxverts") {
            // ignore it
            continue;
        } else if (cmd == "vertex") {
            float x, y, z;
            ss >> x >> y >> z;
            vertices.emplace_back(glm::vec3(x, y, z));
        } else if (cmd == "tri") {
            unsigned int i, j, k;
            ss >> i >> j >> k;
            triangles.emplace_back(triangle(vertices[i], vertices[j], vertices[k]));
            geometries.push_back(&triangles.back());
            triangles.back().ambient_color = ambient_color;
            triangles.back().diffuse_color = diffuse_color;
            triangles.back().specular_color = specular_color;
            triangles.back().emission_color = emission_color;
            triangles.back().shininess = shininess;
            triangles.back().transform = transformStack.top();

        } else if (cmd == "sphere") {
            float x, y, z, r;
            ss >> x >> y >> z >> r;
            spheres.emplace_back(sphere(glm::vec3(x, y, z), r));
            geometries.push_back(&spheres.back());
            spheres.back().ambient_color = ambient_color;
            spheres.back().diffuse_color = diffuse_color;
            spheres.back().specular_color = specular_color;
            spheres.back().emission_color = emission_color;
            spheres.back().shininess = shininess;
            spheres.back().transform = transformStack.top();

        } else
            // TRANSFORMATION
        if (cmd == "translate") {
            float x, y, z;
            ss >> x >> y >> z;
            glm::mat4 tf = transform::translate(x, y, z);
            right_multiply(tf, transformStack);

        } else if (cmd == "rotate") {
            float x, y, z, angle;
            ss >> x >> y >> z >> angle;
            glm::mat4 tf = glm::mat4(transform::rotate(angle, glm::normalize(glm::vec3(x, y, z))));
            right_multiply(tf, transformStack);

        } else if (cmd == "scale") {
            float x, y, z;
            ss >> x >> y >> z;
            glm::mat4 tf = transform::scale(x, y, z);
            right_multiply(tf, transformStack);

        } else if (cmd == "pushTransform") {
            transformStack.push(transformStack.top());
        } else if (cmd == "popTransform") {
            transformStack.pop();
        } else
            // LIGHT
        if (cmd == "attenuation") {
            ss >> attenuation_const >> attenuation_linear >> attenuation_quadratic;
        } else if (cmd == "ambient") {
            float r, g, b;
            ss >> r >> g >> b;
            ambient_color = glm::vec3(r, g, b);
        } else
            // MATERIAL
        if (cmd == "diffuse") {
            float r, g, b;
            ss >> r >> g >> b;
            diffuse_color = glm::vec3(r, g, b);
        } else if (cmd == "specular") {
            float r, g, b;
            ss >> r >> g >> b;
            specular_color = glm::vec3(r, g, b);
        } else if (cmd == "shininess") {
            ss >> shininess;
        } else if (cmd == "emission") {
            float r, g, b;
            ss >> r >> g >> b;
            emission_color = glm::vec3(r, g, b);
        } else {
            std::cerr << "Unrecognized command: " << line << std::endl;
        }
    }
}

void init() {
    max_depth = 5;
    attenuation_const = 1;
    attenuation_linear = 0;
    attenuation_quadratic = 0;
    ambient_color = glm::vec3(0.2, 0.2, 0.2);
}


int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "Usage: raytracer [scene file]" << std::endl;
        return (0);
    }
    init();
    std::string scene_file = std::string(argv[1]);
    read_scene_file(scene_file);

    int byte_size = image_width * image_height * 3;
    uint8_t *result = new uint8_t[byte_size];

    camera::main_camera.render(result);

    FreeImage_Initialise();
    FIBITMAP *img = FreeImage_ConvertFromRawBits(result, image_width, image_height, image_width * 3, 24, 0xFF0000,
                                                 0x00FF00, 0x0000FF, false);
    FreeImage_Save(FIF_PNG, img, output_filename.c_str(), 0);
    FreeImage_DeInitialise();
    delete[] result;
	std::cout << output_filename;
	std::cout << " outputed" << std::endl;
    return 0;
}

