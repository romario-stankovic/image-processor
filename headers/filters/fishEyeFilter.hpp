#pragma once

#include "../misc/color.hpp"
#include "../misc/mathf.hpp"
#include "../misc/types.hpp"
#include "ifilter.hpp"

class FishEyeFilter : public IFilter {
   public:
    FishEyeFilter();
    void apply(Image *image) override;
};

FishEyeFilter::FishEyeFilter(){}

void FishEyeFilter::apply(Image *image) {
    // Get the width and height of the image
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
        // Map x to be in range from -1 to 1
        float ny = Math::map(i, 0, height-1, -1, 1);
        for(int j=0; j<width; j++){

            // Map y to be in range from -1 to 1
            float nx = Math::map(j, 0, width-1, -1, 1);

            PolarCoordinates polar = Math::cart2polar(nx, ny);

            // Modify the polar radius
            float nr = sqrt(1.0f - polar.r*polar.r);
            nr = (polar.r + (1.0f-nr)) / 2.0;

            // Convert back from polar
            CartesianCoordinates cart = Math::polar2cart(nr, polar.theta);
            int x2 = Math::map(cart.x, -1, 1, 0, width-1);
            int y2 = Math::map(cart.y, -1, 1, 0, height-1);

            // Check if x2 and y2 are in the range of the image
            if(x2 < 0 || x2 > width-1 || y2 < 0 || y2 > height-1){
                // If not, set these pixels to black
                (*image).setPixel(j, i, {0,0,0});
                continue;
            }

            // Set the pixel to the new position
            (*image).setPixel(j, i, copy[y2][x2]);

        }
    }

    for (int i = 0; i < height; i++) {
        delete[] copy[i];
    }
    delete[] copy;
}