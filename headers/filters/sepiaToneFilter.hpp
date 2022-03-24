#pragma once

#include "ifilter.hpp"

#include "../image/image.hpp"
#include "../misc/mathf.hpp"
#include "../misc/types.hpp"

class SepiaToneFilter : public IFilter {

    private:
    float intensity;

    public:
        SepiaToneFilter();
        SepiaToneFilter(float intensity);
        void apply(Image *image) override;
};

SepiaToneFilter::SepiaToneFilter() {
    this->intensity = 1;
};

SepiaToneFilter::SepiaToneFilter(float intensity){
    // Set intensity and don't allow it to go bellow 0 and above 1
    this->intensity = Math::clamp(intensity, 0, 1);
}

void SepiaToneFilter::apply(Image *image){

    // Read image width and height
    int width = (*image).getDimensions().width;
    int height = (*image).getDimensions().height;

    // Loop through the pixels
    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            // Get the pixel value from the image
            Color pixel = (*image).getPixel(j, i);

            // Calculate new values for RGB and clamp them between 0 and 255
            uint8 red = Math::clamp((pixel.red * 0.393 + pixel.green * 0.769 + pixel.blue * 0.189), 0, 255);
            uint8 green = Math::clamp((pixel.red * 0.349 + pixel.green * 0.686 + pixel.blue * 0.168), 0, 255);
            uint8 blue = Math::clamp((pixel.red * 0.272 + pixel.green * 0.534 + pixel.blue * 0.131), 0, 255);

            // Lerp between default color and our new color based on sepia tone intensity
            Color color = Color::lerp(pixel, {red, green, blue, pixel.alpha}, intensity);

            // Set the pixel value of the image
            (*image).setPixel(j, i, color);
            
        }
    }

}