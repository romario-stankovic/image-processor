#pragma once

#include "ifilter.hpp"
#include "../misc/color.hpp"
#include "../misc/mathf.hpp"
#include "../misc/types.hpp"

class ScanlineFilter : public IFilter {
    private:
    float intensity;
    public:
    ScanlineFilter();
    ScanlineFilter(float intensity);
    void apply(Image *image) override;
};

ScanlineFilter::ScanlineFilter(){
    this->intensity = 1;
}

ScanlineFilter::ScanlineFilter(float intensity){
    // Limit the intensity of scanlines between 0 and 1
    this->intensity = Math::clamp(intensity, 0, 1);
}

void ScanlineFilter::apply(Image *image){
    // Get width and height from the image
    int width = (*image).getDimensions().width;
    int height = (*image).getDimensions().height;

    // Loop through each pixel
    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            // Get the color of the pixel
            Color pixel = (*image).getPixel(j,i);

            // If we are a even row, set the color to black, else leave the color
            Color color = i % 2 == 0 ? Color{0,0,0} : pixel;

            // Lerp the color and set the new color onto the image
            (*image).setPixel(j, i, Color::lerp(pixel, color, intensity));

        }
    }

}