#pragma once

#include <cmath>

struct PolarCoordinates {
    float r;
    float theta;
};

struct CartesianCoordinates{
    float x;
    float y;
};

class Math {
   public:
    static float lerp(float start, float end, float percentage);
    static float clamp(float value, float min, float max);
    static float map(float input, float fromStart, float fromEnd, float toStart, float toEnd);
    static PolarCoordinates cart2polar(float x, float y);
    static CartesianCoordinates polar2cart(float r, float theta);
};

// Linear interpolation
float Math::lerp(float start, float end, float percentage) {
    return start + (end - start) * percentage;
}

// Limit value betwen two numbers
float Math::clamp(float value, float min, float max) {

    if (value < min) {
        value = min;
    }

    if (value > max) {
        value = max;
    }
    return value;
}

float Math::map(float input, float fromStart, float fromEnd, float toStart, float toEnd){
    return toStart + (input - fromStart) * (toEnd - toStart) / (fromEnd - fromStart);
}

PolarCoordinates Math::cart2polar(float x, float y){
    float r = sqrt(x*x + y*y);
    float theta = atan2(y, x);
    return {r, theta};
}

CartesianCoordinates Math::polar2cart(float r, float theta){
    float x = r * cos(theta);
    float y = r * sin(theta);
    return {x, y};
}