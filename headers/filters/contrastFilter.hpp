#pragma once

#include "ifilter.hpp"
#include "../misc/color.hpp"
#include "../misc/mathf.hpp"
#include "../misc/types.hpp"

class ContrastFilter : public IFilter {

    private:
    float contrast;

    public:
    ContrastFilter();
    ContrastFilter(int contrast);

    void apply(Image *image) override;

};

ContrastFilter::ContrastFilter(){
    this->contrast = 1;
}

ContrastFilter::ContrastFilter(int contrast){
    this->contrast = Math::clamp(contrast, -255, 255);
}

void ContrastFilter::apply(Image *image){

    float factor = (259 * (contrast + 255))/(255 * (259 - contrast));

    // Get the width and height of the image
    int width = (*image).getDimensions().width;
    int height = (*image).getDimensions().height;

    // Loop throught the pixels
    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            // Get the color from the image
            Color pixel = (*image).getPixel(j, i);
            // Apply contrast
            uint8 red = Math::clamp(factor * (pixel.red - 128) + 128, 0, 255);
            uint8 green = Math::clamp(factor * (pixel.green - 128) + 128, 0, 255);
            uint8 blue = Math::clamp(factor * (pixel.blue - 128) + 128, 0, 255);
            // Save the color back onto the image
            (*image).setPixel(j, i, {red, green, blue, pixel.alpha});
        }
    }

}