#pragma once

#include "ifilter.hpp"
#include "../misc/color.hpp"
#include "../misc/mathf.hpp"
#include "../misc/types.hpp"

class ColorFilter : public IFilter {
    private:
    float redChannel;
    float greenChannel;
    float blueChannel;

    public:
    ColorFilter();
    ColorFilter(float redChannel, float greenChannel, float blueChannel);

    void apply(Image *image) override;
};

ColorFilter::ColorFilter(){
    redChannel = 1;
    greenChannel = 1;
    blueChannel = 1;
}

ColorFilter::ColorFilter(float redChannel, float greenChannel, float blueChannel){
    
    // Set the range for each color
    this->redChannel = Math::clamp(redChannel, 0, 1);
    this->greenChannel = Math::clamp(greenChannel, 0, 1);
    this->blueChannel = Math::clamp(blueChannel, 0, 1);
}

void ColorFilter::apply(Image *image){

    // Get width and height from the image
    int width = (*image).getDimensions().width;
    int height = (*image).getDimensions().height;

    // Loop throught each pixel
    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            // Get pixel value from the image
            Color pixel = (*image).getPixel(j,i);
            // Multiply the color with the channel value
            pixel.red = pixel.red * redChannel;
            pixel.green = pixel.green * greenChannel;
            pixel.blue = pixel.blue * blueChannel;
            // Set pixel value on the image
            (*image).setPixel(j, i, pixel);
        }
    }

}