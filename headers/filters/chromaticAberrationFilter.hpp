#pragma once

#include "../misc/color.hpp"
#include "../misc/mathf.hpp"
#include "../misc/types.hpp"
#include "ifilter.hpp"

class ChromaticAberrationFilter : public IFilter {
   private:
    int displacement;

   public:
    ChromaticAberrationFilter();
    ChromaticAberrationFilter(int displacement);
    void apply(Image *image) override;
};

ChromaticAberrationFilter::ChromaticAberrationFilter() {
    displacement = 5;
}

ChromaticAberrationFilter::ChromaticAberrationFilter(int displacement) {
    // Set the displacement of the chromatic abberation
    this->displacement = displacement;
}

void ChromaticAberrationFilter::apply(Image *image) {
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

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {

            // Get the color from the copy
            Color pixel = copy[i][j];

            // Get displaced pixel position for red and blue
            int x = Math::clamp(j - displacement, 0, width-1);
            int nx = Math::clamp(j + displacement, 0, width-1);

            // Set the current pixel red and blue to the displaced red and blue pixels
            pixel.red = copy[i][x].red;
            pixel.blue = copy[i][nx].blue;

            // Set the values back onto the image
            (*image).setPixel(j, i, pixel);

        }
    }

    // Free memory allocated by our copy

    for (int i = 0; i < height; i++) {
        delete[] copy[i];
    }
    delete[] copy;
}