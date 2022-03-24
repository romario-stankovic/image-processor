#pragma once

#include "ifilter.hpp"
#include "../misc/color.hpp"
#include "../misc/mathf.hpp"
#include "../misc/types.hpp"
#include <cmath>
#define M_PI 3.14159265358979323846

// A structure that stores unclamped color values
struct UnboundColor {
    int red = 0;
    int green = 0;
    int blue = 0;
};

class GaussianBlurFilter : public IFilter {
    private:
        int radius;
        float **kernel;
        void generateKernel();
    public:
    GaussianBlurFilter();
    GaussianBlurFilter(int size);

    void apply(Image *image) override;
};

GaussianBlurFilter::GaussianBlurFilter(){
    this->radius = 5;
}

GaussianBlurFilter::GaussianBlurFilter(int radius){
    // Set radius and clamp it to a positive number
    this->radius = Math::clamp(radius, 0, INT_MAX);
}

void GaussianBlurFilter::generateKernel(){

    // Calculate size and set center point of the kernel
    int size = 2 * radius + 1;
    int center = radius;


    // Create kernel 2d array
    kernel = new float *[size];
    for(int i=0; i<size; i++){
        kernel[i] = new float[size];
    }

    // Calculate constant values for gaussian blur formula
    float sigma = (float)radius/2;
    float alpha = 1.0f/(sqrt(2 * M_PI) * sigma);
    float beta = (2 * pow(sigma, 2));

    // Using gaussian formula, calculate the values for the kernel pixels
    float max = 0;
    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            float x = exp( - pow(j - center, 2) / beta);
            float y = exp( - pow(i - center, 2) / beta);
            float mul = alpha * alpha * x * y;
            if(mul >= max){
                max = mul;
            }
            kernel[i][j] = mul;
        }
    }

    // Normalize kernel pixel values so they span between 0 and 1
    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            kernel[i][j] /= max;
        }
    }

}

void GaussianBlurFilter::apply(Image *image){

    // If we have a radius of 0, we don't need to apply the filter
    if(radius == 0){
        return;
    }

    // Generate the kernel
    generateKernel();

    // Calculate the kernel size and center point
    int size = 2 * radius + 1;
    int half = size / 2;

    // Get width and height from the image
    int width = (*image).getDimensions().width;
    int height = (*image).getDimensions().height;

    // Create a new 2d array that stores the convolved image data
    UnboundColor **temp = new UnboundColor*[height];
    for(int i=0; i<height; i++){
        temp[i] = new UnboundColor[width];
    }

    // Convolve the image and kernel
    int max = 1;
    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){

            for (int ky = 0; ky < size; ky++) {
                for (int kx = 0; kx < size; kx++) {
                    int x = j + kx - half;
                    int y = i + ky - half;
                    if (x < 0 || y < 0 || y >= height || x >= width) {
                        continue;
                    }
                    // Sum up all the colors
                    temp[i][j].red += (*image).getPixel(x,y).red * kernel[ky][kx];
                    temp[i][j].green += (*image).getPixel(x,y).green * kernel[ky][kx];
                    temp[i][j].blue += (*image).getPixel(x,y).blue * kernel[ky][kx];
                }
            }

            // Check which color has the highest value
            if(temp[i][j].red > max){
                max = temp[i][j].red;
            }
            if(temp[i][j].green > max){
                max = temp[i][j].green;
            }
            if(temp[i][j].blue > max){
                max = temp[i][j].blue;
            }
        }
    }

    // Divide all colors by max so we get values in the range from 0 to 1 and multiply them with 255
    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            uint8 red = ((float)temp[i][j].red / max) * 255;
            uint8 green = ((float)temp[i][j].green / max) * 255;
            uint8 blue = ((float)temp[i][j].blue / max) * 255;
            (*image).setPixel(j, i, {red, green, blue});
        }
    }

    // Free memory allocated by our temporary array
    for(int i=0; i<height; i++){
        delete[] temp[i];
    }
    delete[] temp;

    // Free memory allocated by our kernel
    for(int i=0; i<size; i++){
        delete[] kernel[i];
    }
    delete[] kernel;

}