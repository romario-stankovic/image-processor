#pragma once

#include "ifilter.hpp"
#include "../misc/color.hpp"
#include "../misc/mathf.hpp"
#include "../misc/types.hpp"

class GrayscaleFilter : public IFilter {
    private:
    float intensity;

    public:
    GrayscaleFilter();
    GrayscaleFilter(float intensity);

    void apply(Image *image) override;
};

GrayscaleFilter::GrayscaleFilter(){
    this->intensity = 1;
}

GrayscaleFilter::GrayscaleFilter(float intensity){
    // Don't allow the intensity to go bellow 0 and above 1
    this->intensity =  Math::clamp(intensity, 0, 1);
}

void GrayscaleFilter::apply(Image *image){

    // Get the width and height of the image
    int width = (*image).getDimensions().width;
    int height = (*image).getDimensions().height;

    // Loop throught the pixels
    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            // Get the pixel from the image
            Color pixel = (*image).getPixel(j,i);

            // Calculate the gray value
            uint8 value = (pixel.red * 0.299) + (pixel.green * 0.587) + (pixel.blue*0.114);

            // Lerp between the colors based on intensity
            Color color = Color::lerp(pixel, {value, value, value, pixel.alpha}, intensity);

            // Set the pixel value on the image
            (*image).setPixel(j, i, color);
        }
    }

}