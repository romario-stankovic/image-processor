#pragma once

#include <cmath>

#include "../misc/color.hpp"
#include "../misc/mathf.hpp"
#include "../misc/types.hpp"
#include "ifilter.hpp"
#define M_PI 3.14159265358979323846

class AverageBlurFilter : public IFilter {
   private:
    int radius;

   public:
    AverageBlurFilter();
    AverageBlurFilter(int size);

    void apply(Image *image) override;
};

AverageBlurFilter::AverageBlurFilter() {
    this->radius = 5;
}

AverageBlurFilter::AverageBlurFilter(int radius) {
    // Clamp the input radius to a positive number
    this->radius = Math::clamp(radius, 0, INT_MAX);
}

void AverageBlurFilter::apply(Image *image) {
    // If our radius is 0, we don't need to apply the filter
    if(radius == 0){
        return;
    }

    // Calculate size
    int size = 2 * radius - 1;
    int half = size/2;
    
    // Get width and height from the image
    int width = (*image).getDimensions().width;
    int height = (*image).getDimensions().height;

    // Create a copy of our base image
    Color **copy = new Color*[height];
    for(int i=0; i<height; i++){
        copy[i] = new Color[width];
        for(int j=0; j<width; j++){
            copy[i][j] = (*image).getPixel(j,i);
        }
    }

    // Loop through the pixels
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            // Set counters
            int sumR = 0;
            int sumG = 0;
            int sumB = 0;
            int count = 0;

            // Sum up all values
            for (int k = 0; k < size; k++) {
                for (int l = 0; l < size; l++) {
                    int x = j + l - half;
                    int y = i + k - half;
                    if (x < 0 || y < 0 || x >= width || y >= height) {
                        continue;
                    }
                    sumR += copy[y][x].red;
                    sumG += copy[y][x].green;
                    sumB += copy[y][x].blue;
                    count++;
                }
            }
            // Calculate the average for the colors
            uint8 red = (sumR / count);
            uint8 green = (sumG / count);
            uint8 blue = (sumB / count);

            // Set pixel value to our averages
            (*image).setPixel(j, i, {red, green, blue});
        }
    }

    // Free memory allocated by our copy

    for(int i=0; i<height; i++){
        delete[] copy[i];
    }
    delete[] copy;

}