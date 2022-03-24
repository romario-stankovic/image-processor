#pragma once

#include "../misc/color.hpp"
#include "../misc/mathf.hpp"
#include "../misc/types.hpp"
#include "ifilter.hpp"

class PixelateFilter : public IFilter {
    private:
        int size;
    public:
    PixelateFilter();
    PixelateFilter(int size);
    void apply(Image *image) override;
};

PixelateFilter::PixelateFilter(){
    size = 5;
}

PixelateFilter::PixelateFilter(int size){
    this->size = Math::clamp(size, 0, INT_MAX);
}

void PixelateFilter::apply(Image *image){
    if(size == 0){
        return;
    }
    
    // Get width and height from the image
    int width = (*image).getDimensions().width;
    int height = (*image).getDimensions().height;
    // Create a copy of our base image
    Color **copy = new Color *[height];
    for (int i = 0; i < height; i++) {
        copy[i] = new Color[width];
        for (int j = 0; j < width; j++) {
            copy[i][j] = (*image).getPixel(j, i);
        }
    }

    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            int x = size * (j / size);
            int y = size * (i / size);
            Color pixel = copy[y][x];
            (*image).setPixel(j, i, pixel);
        }
    }

    // Free memory allocated by our copy

    for(int i=0; i<height; i++){
        delete[] copy[i];
    }
    delete[] copy;

}