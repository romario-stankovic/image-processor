#pragma once

#include "ifilter.hpp"
#include "../misc/color.hpp"
#include "../misc/mathf.hpp"
#include "../misc/types.hpp"
#include "grayscaleFilter.hpp"

class EdgeDetectionFilter : public IFilter {
    private:
        float **kernel;
    public:
    EdgeDetectionFilter();

    void apply(Image *image) override;
};

EdgeDetectionFilter::EdgeDetectionFilter(){
    this->kernel = new float*[3];
    for(int i=0; i<3; i++){
        kernel[i] = new float[3];
    }

    kernel[0][0] = -0.125;
    kernel[0][1] = -0.125;
    kernel[0][2] = -0.125;

    kernel[1][0] = -0.125;
    kernel[1][1] = 1;
    kernel[1][2] = -0.125;

    kernel[2][0] = -0.125;
    kernel[2][1] = -0.125;
    kernel[2][2] = -0.125;

}

void EdgeDetectionFilter::apply(Image *image){

    // Apply a grayscale filter before calculating the edges
    GrayscaleFilter filter;
    filter.apply(image);

    // Set the size of the kernel
    int size = 3;
    int half = size / 2;

    // Get width and height from the image
    int width = (*image).getDimensions().width;
    int height = (*image).getDimensions().height;
    // Allocate memory to store the convolved values for each pixel
    float **temp = new float*[height];
    for(int i=0; i<height; i++){
        temp[i] = new float[width];
        for(int j=0; j<width; j++){
            temp[i][j] = 0;
        }
    }

    // Convolve the image with the kernel
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
                    temp[i][j] += (*image).getPixel(x,y).red * kernel[ky][kx];
                }
            }
            // Get the maximum value from the convolution
            if(temp[i][j] > max){
                max = temp[i][j];
            }
        }
    }

    // Clamp the value for each pixel between 0 and 1, then multiply it with 255
    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            uint8 value = Math::clamp((float)(temp[i][j]) / max, 0, 1) * 255;
            (*image).setPixel(j, i, {value, value, value});
        }
    }

    // Free memory allocated by our temporary array
    for(int i=0; i<height; i++){
        delete[] temp[i];
    }
    delete[] temp;

    // Free memory allocated by our kernel
    for(int i=0; i<3; i++){
        delete[] kernel[i];
    }
    delete[] kernel;

}