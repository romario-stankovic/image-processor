#pragma once

#include "ifilter.hpp"
#include "../misc/color.hpp"
#include "../misc/mathf.hpp"
#include "../misc/types.hpp"

class BrightnessFilter : public IFilter {

    private:
    int brightness;

    public:
    BrightnessFilter();
    BrightnessFilter(int brightness);

    void apply(Image *image) override;

};

BrightnessFilter::BrightnessFilter(){
    this->brightness = 10;
}

BrightnessFilter::BrightnessFilter(int brightness){
    this->brightness = brightness;
}

void BrightnessFilter::apply(Image *image){
    // Add the value of brightness onto the image
    (*image) += brightness;
}