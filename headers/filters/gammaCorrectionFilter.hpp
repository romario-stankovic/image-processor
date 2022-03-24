#pragma once

#include "../misc/color.hpp"
#include "../misc/mathf.hpp"
#include "../misc/types.hpp"
#include "ifilter.hpp"

class GammaCorrectionFilter : public IFilter {
   private:
    float gamma;

   public:
    GammaCorrectionFilter();
    GammaCorrectionFilter(float gamma);

    void apply(Image *image) override;
};

GammaCorrectionFilter::GammaCorrectionFilter() {
    this->gamma = 1;
}

GammaCorrectionFilter::GammaCorrectionFilter(float gamma) {
    this->gamma = Math::clamp(gamma, 0, 10);
}

void GammaCorrectionFilter::apply(Image *image) {
    float correction = 1 / gamma;

    // Get the width and height of the image
    int width = (*image).getDimensions().width;
    int height = (*image).getDimensions().height;

    // Loop throught the pixels
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            // Get the pixel from the image
            Color pixel = (*image).getPixel(j, i);
            // Apply gamma correction
            uint8 red = 255 * pow((pixel.red / 255.0f), correction);
            uint8 green = 255 * pow((pixel.green / 255.0f), correction);
            uint8 blue = 255 * pow((pixel.blue / 255.0f), correction);
            // Set the pixel back
            (*image).setPixel(j, i, {red, green, blue, pixel.alpha});
        }
    }
}