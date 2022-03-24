#pragma once

#include <fstream>
#include <string>

#include "image.hpp"
#include "../misc/types.hpp"
#include "../misc/color.hpp"
#include "../misc/exceptions.hpp"

#pragma pack(push, 1)
struct BMPFileHeader {
    uint16 fileType = 0x4D42;  // BMP have a file type of 'BM' which is (4D,42) hex
    uint32 fileSize = 0;       // The size of the file in bytes
    uint16 reserved1 = 0;      // Reserved, always 0
    uint16 reserved2 = 0;      // Reserved, always 0
    uint32 offset = 0;         // Offset of pixel data in bytes from the start of the file
};

struct BMPInfoHeader {
    uint32 size = 0;   // Size of the header
    int32 width = 0;   // Width of the image
    int32 height = 0;  // Height of the image (positive -> image saved backwards, negative -> image saved as is)

    uint16 planes = 1;       // Number of planes
    uint16 depth = 0;        // Bits depth per pixel
    uint32 compression = 0;  // 0 or 3
    uint32 imageSize = 0;    // 0 for uncompressed images
    int32 xPixelsPerMeter = 0;
    int32 yPixelsPerMeter = 0;
    uint32 colorsUsed = 0;       // Number of colors indexed in the color table. 0 is max number of colors
    uint32 colorsImportant = 0;  // Number of colors used to display the image. 0 is all colors
};

struct BMPColorHeader {
    uint32 redMask = 0x00ff0000;
    uint32 greenMask = 0x0000ff00;
    uint32 blueMask = 0x000000ff;
    uint32 alphaMask = 0xff000000;
    uint32 colorSpace = 0x73524742;  // Code for sRGB color space
    uint32 unused[16];
};
#pragma pack(pop)

class BMPImage : public Image {
   private:
    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;
    BMPColorHeader colorHeader;
    bool validColorHeader();

   public:
    BMPImage();
    ~BMPImage();
    
    void load(string path) override;
    void write(string path) override;
    void create(uint32 width, uint32 height) override;
    void unload() override;
};

// Constructor
BMPImage::BMPImage() {
    width = 0;
    height = 0;
    image = nullptr;
}

// Destructor
BMPImage::~BMPImage() {
    unload();
}

// File reader
void BMPImage::load(string path) {
    // Create a input file stream
    ifstream file;
    file.open(path.c_str(), ios_base::binary);

    // If we failed to load the file, throw an exception
    if (!file) {
        throw FileNotFoundException();
    }

    // Read the file header
    file.read((char *)&fileHeader, sizeof(fileHeader));

    // If we are reading the wrong file format, throw an exception
    if (fileHeader.fileType != 0x4D42) {
        throw WrongFormatException();
    }

    // Read the info header
    file.read((char *)&infoHeader, sizeof(infoHeader));

    if (infoHeader.depth == 32) {
        // If we have a 32bit bitmap, load color header
        file.read((char *)&colorHeader, sizeof(colorHeader));

        if(!validColorHeader()){
            throw BadFileFormatException();
        }
    }

    // Discard any extra information from the headers placed by image editors
    infoHeader.size = sizeof(BMPInfoHeader) + (infoHeader.depth == 32 ? sizeof(BMPColorHeader) : 0);
    fileHeader.offset = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + (infoHeader.depth == 32 ? sizeof(BMPColorHeader) : 0);
    fileHeader.fileSize = fileHeader.offset;

    // Go to the pixel data location
    file.seekg(fileHeader.offset, file.beg);

    // Read width, height and check if the data is flipped
    bool invertHeight = infoHeader.height < 0;
    width = infoHeader.width;
    height = invertHeight ? -infoHeader.height : infoHeader.height;

    //Unload any previous image
    unload();
    // Create a matrix for storing our pixels
    image = new Color *[height];
    for (int i = 0; i < height; i++) {
        image[i] = new Color[width];
    }

    // Make an array that will store the data read from the file
    uint8 *data;

    // Row Width represents the amount of bytes that need to be read to fill one row of the image
    int rowWidth = 0;

    // 24-bit bitmap
    if (infoHeader.depth == 24) {
        // Row width = number of pixels * 3 colors (r,g,b) + padding

        // Calculate padding
        int byteWidth = width * 3;
        int newByteWidth = byteWidth;
        while (newByteWidth % 4 != 0) {
            newByteWidth++;
        }
        rowWidth = newByteWidth - (width % 4 != 0 ? 3 : 0);

        // Allocate array to store row
        data = new uint8[rowWidth];

        // Read data from file
        for (int i = 0; i < height; i++) {
            file.read((char *)data, rowWidth);
            for (int j = 0; j < width * 3; j += 3) {
                // Read RGB data
                uint8 blue = data[j];
                uint8 green = data[j + 1];
                uint8 red = data[j + 2];
                // Check if the image is inverted and save it in the correct oreder
                if (invertHeight) {
                    image[i][j / 3] = {red, green, blue, 255};
                } else {
                    image[height - 1 - i][j / 3] = {red, green, blue, 255};
                }
            }
        }
    }

    // 32-bit bitmap
    if(infoHeader.depth == 32) {
        // Row width = number of pixels * 4 colors (r,g,b,a)
        rowWidth = (4 * width);
        // Allocate array to store row
        data = new uint8[rowWidth];

        // Read data from file
        for(int i=0; i<height; i++){
            file.read((char*)data, rowWidth);
            for(int j=0; j<width*4; j+=4){
                
                // Read RGBA data
                uint8 blue = data[j];
                uint8 green = data[j + 1];
                uint8 red = data[j + 2];
                uint8 alpha = data[j+3];

                // Check if the image is inverted and save it in the correct oreder
                if (invertHeight) {
                    image[i][j / 4] = {red, green, blue, alpha};
                } else {
                    image[height - 1 - i][j / 4] = {red, green, blue, alpha};
                }
            }
        }

    }

    // Increase the size of the file
    fileHeader.fileSize += rowWidth * height;

    // Release pointers
    file.close();
    delete[] data;
}

// File writer
void BMPImage::write(string path) {

    // Check if we have image data, if not don't save the image
    if(image == nullptr){
        return;
    }

    // Create a output file stream
    ofstream file;
    file.open(path, ios_base::binary);

    // If we failed to create the file, throw an exception
    if (!file) {
        throw WriteException();
    }

    // Write the headers
    file.write((const char *)&fileHeader, sizeof(fileHeader));
    file.write((const char *)&infoHeader, sizeof(infoHeader));
    if (infoHeader.depth == 32) {
        file.write((const char *)&colorHeader, sizeof(colorHeader));
    }

    // Check if we need to write the image right side up, or upside down

    bool invertHeight = infoHeader.height < 0;

    // Create a pointer that points to a specific pixel
    Color *pixel;

    // Write all pixels to the file
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            pixel = &(invertHeight ? image[i][j] : image[height - 1 - i][j]);

            file.write((const char *)&(*pixel).blue, sizeof(uint8));
            file.write((const char *)&(*pixel).green, sizeof(uint8));
            file.write((const char *)&(*pixel).red, sizeof(uint8));
            if (infoHeader.depth == 32) {
                file.write((const char *)&(*pixel).alpha, sizeof(uint8));
            }
        }
    }

    // Close the file
    file.close();
}

void BMPImage::create(uint32 width, uint32 height){
    // Create headers
    BMPColorHeader ch = {
        0x00ff0000,
        0x0000ff00,
        0x000000ff,
        0xff000000,
        0x73524742
    };

    BMPInfoHeader ih = {
        0,
        (int32)width,
        (int32)height,
        1,
        32,
        0,
        0,
        11700,
        11700,
        0,
        0
    };

    BMPFileHeader fh = {
        0x4D42,
        0,
        0,
        0,
        0
    };

    // Set header values
    ih.size = sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
    fh.offset = sizeof(BMPFileHeader) + ih.size;
    fh.fileSize = fileHeader.offset + (4 * width * height);

    // Assign headers
    this->fileHeader = fh;
    this->infoHeader = ih;
    this->colorHeader = ch;

    // Create image using the base class
    Image::create(width, height);
}

// Dealocator for the image
void BMPImage::unload() {
    Image::unload();
}

bool BMPImage::validColorHeader(){
    
    if(infoHeader.size < (sizeof(BMPInfoHeader) + sizeof(BMPColorHeader))){
        // We don't have a color header
        return false;
    }
    // Check is the color header valid
    BMPColorHeader expectedHeader;
    if (colorHeader.redMask != expectedHeader.redMask ||
        colorHeader.greenMask != expectedHeader.greenMask ||
        colorHeader.blueMask != expectedHeader.blueMask ||
        colorHeader.alphaMask != expectedHeader.alphaMask) {
        // Bad color mask format
        return false;
    }

    if (colorHeader.colorSpace != expectedHeader.colorSpace) {
        // Bad color format
        return false;
    }

    return true;
}