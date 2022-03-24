#pragma once

#include "../image/image.hpp"

// Filter interface
class IFilter {
   public:
    virtual void apply(Image *image) = 0;
};