#pragma once

#include <string>

#include "../misc/color.hpp"
#include "../misc/types.hpp"

using namespace std;

// Structure that represent the size of the image
struct Size {
    uint32 width;
    uint32 height;
};

class Image {
   protected:
    uint32 width;
    uint32 height;
    Color **image;

   public:
    virtual void load(string path) = 0;
    virtual void write(string path) = 0;
    virtual void create(uint32 width, uint32 height);
    virtual void unload();

    Size getDimensions();
    void setPixel(uint32 x, uint32 y, Color color);
    Color getPixel(uint32 x, uint32 y);

    void operator+=(int value);
    void operator-=(int value);
    void operator*=(float value);
    void operator/=(float value);
};

void Image::create(uint32 width, uint32 height) {
    // Unload any previous image
    unload();
    // Set width and height
    this->width = width;
    this->height = height;

    // Allocate new memory for the image
    image = new Color *[height];
    for (int i = 0; i < height; i++) {
        image[i] = new Color[width];
    }
}

// Dealocator for the image matrix
void Image::unload() {
    // Check if we have a image to unload, if not return
    if (image == nullptr) {
        return;
    }

    // Unload columns
    for (int i = 0; i < height; i++) {
        delete[] image[i];
    }
    // Unload rows
    delete[] image;
    // Set image to null
    image = nullptr;
}

// Getter for image size (width, height)
Size Image::getDimensions() {
    return {width, height};
}

// Set pixel color at (x,y)
void Image::setPixel(uint32 x, uint32 y, Color color) {
    image[y][x] = color;
}

// Get pixel color at (x,y)
Color Image::getPixel(uint32 x, uint32 y) {
    return image[y][x];
}

// Operators

void Image::operator+=(int value) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            uint8 red = Math::clamp(image[i][j].red + value, 0, 255);
            uint8 green = Math::clamp(image[i][j].green + value, 0, 255);
            uint8 blue = Math::clamp(image[i][j].blue + value, 0, 255);
            uint8 alpha = image[i][j].alpha;
            image[i][j] = {red, green, blue, alpha};
        }
    }
}

void Image::operator-=(int value){
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            uint8 red = Math::clamp(image[i][j].red - value, 0, 255);
            uint8 green = Math::clamp(image[i][j].green - value, 0, 255);
            uint8 blue = Math::clamp(image[i][j].blue - value, 0, 255);
            uint8 alpha = image[i][j].alpha;
            image[i][j] = {red, green, blue, alpha};
        }
    }
}

void Image::operator*=(float value) {
    if (value < 0) {
        return;
    }

    // When multiplying the value, we multiply each color separately

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            uint8 red = Math::clamp(image[i][j].red * value, 0, 255);
            uint8 green = Math::clamp(image[i][j].green * value, 0, 255);
            uint8 blue = Math::clamp(image[i][j].blue * value, 0, 255);
            uint8 alpha = image[i][j].alpha;
            image[i][j] = {red, green, blue, alpha};
        }
    }
}

void Image::operator/=(float value) {
    if (value <= 0) {
        return;
    }

    // When dividing the values, we divide each color with the given number

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            uint8 red = Math::clamp(image[i][j].red / value, 0, 255);
            uint8 green = Math::clamp(image[i][j].green / value, 0, 255);
            uint8 blue = Math::clamp(image[i][j].blue / value, 0, 255);
            uint8 alpha = image[i][j].alpha;
            image[i][j] = {red, green, blue, alpha};
        }
    }
}