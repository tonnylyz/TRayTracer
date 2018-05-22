//
// Created by Tonny on 4/7/2018.
//

#ifndef RAYTRACER_STRING_UTIL_H
#define RAYTRACER_STRING_UTIL_H


#include <string>
#include <algorithm>
#include <cctype>

static inline void left_trim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

static inline void right_trim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

static inline void trim(std::string &s) {
    left_trim(s);
    right_trim(s);
}

#endif //RAYTRACER_STRING_UTIL_H
