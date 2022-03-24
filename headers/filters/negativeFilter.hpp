#pragma once

#include "ifilter.hpp"
#include "../image/image.hpp"

class NegativeFilter : public IFilter {

    public:
    NegativeFilter();
    void apply(Image *image) override;

};

NegativeFilter::NegativeFilter(){};

void NegativeFilter::apply(Image * image){

    // Read image width and height
    int width = (*image).getDimensions().width;
    int height = (*image).getDimensions().height;

    // Loop through the pixels
    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            // Get the pixel value from the image
            Color pixel = (*image).getPixel(j, i);

            // Invert the pixel value
            pixel.red = 255 - pixel.red;
            pixel.green = 255 - pixel.green;
            pixel.blue = 255 - pixel.blue;

            // Set the pixel value on the image
            (*image).setPixel(j, i, pixel);
        }
    }

}