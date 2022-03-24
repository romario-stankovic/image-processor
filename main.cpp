#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "headers/filters/averageBlurFilter.hpp"
#include "headers/filters/brightnessFilter.hpp"
#include "headers/filters/chromaticAberrationFilter.hpp"
#include "headers/filters/colorFilter.hpp"
#include "headers/filters/contrastFilter.hpp"
#include "headers/filters/edgeDetectionFilter.hpp"
#include "headers/filters/fishEyeFilter.hpp"
#include "headers/filters/gammaCorrectionFilter.hpp"
#include "headers/filters/gaussianBlurFilter.hpp"
#include "headers/filters/grayscaleFilter.hpp"
#include "headers/filters/negativeFilter.hpp"
#include "headers/filters/scanlineFilter.hpp"
#include "headers/filters/sepiaToneFilter.hpp"
#include "headers/filters/pixelateFilter.hpp"
#include "headers/image/bmpimage.hpp"
#include "headers/menu.hpp"

using namespace std;

bool startsWith(string str, string comp);
int getPresetInt(string str);
float getPresetFloat(string str);

void loadMenu();
void editorMenu();
void saveMenu(bool enterName);
void filterMenu();
void presetMenu();

void colorNegativeFilter(bool fromPreset);
void grayscale(bool fromPreset, float intensity);
void sepiaTone(bool fromPreset, float intensity);
void colorFilter(bool fromPreset, float r, float g, float b);
void averageBlur(bool fromPreset, int radius);
void gaussianBlur(bool fromPreset, int radius);
void edgeDetection(bool fromPreset);
void chromaticAberration(bool fromPreset, int displacement);
void brightness(bool fromPreset, int intensity);
void contrast(bool fromPreset, int intensity);
void gammaCorrection(bool fromPreset, float value);
void scanlines(bool fromPreset, float intensity);
void fishEye(bool fromPreset);
void pixelate(bool fromPreset, int size);

void quit(int code);

Image *image;

string saveName = "";

bool startsWith(string str, string comp) {
    return (str.rfind(comp, 0) == 0);
}

int getPresetInt(string str) {
    int index = str.find(':');
    return stoi(str.substr(index + 1));
}

float getPresetFloat(string str) {
    int index = str.find(':');
    return stof(str.substr(index + 1));
}

int main() {
    // Items to be displayed to the user
    vector<string> options;
    options.insert(options.end(), "Load image");
    options.insert(options.end(), "Exit");
    // Display the menu and ask the user for input
    int selection = -1;
    do {
        selection = menu(options, "Menu:");
        switch (selection) {
            case 0:
                loadMenu();
                break;
            case 1:
                quit(0);
                break;
        }
        selection = -1;
    } while (selection == -1);

    return 0;
}

void loadMenu() {
    clearScreen();

    string path;
    FILE *file;

    cout << "Load: \n\n";

    // Try to open a file pointer where the image is at
    // If we fail to open the file, try asking for another path
    do {
        cout << "Enter the path to a bitmap image or (cancel): \n";
        cin >> path;
        file = fopen(path.c_str(), "r");
    } while (!file && path != "cancel");
    fclose(file);
    // If we successfully opened the file, close it and load the image
    // from the path provided

    BMPImage img;
    try {
        img.load(path);
        image = &img;
    } catch (exception e) {
        return;
    }

    editorMenu();

    // When the editor closes, we unload the image from memory
    (*image).unload();
    saveName = "";
}

void editorMenu() {
    vector<string> options;
    options.insert(options.end(), "Apply Filter");
    options.insert(options.end(), "Apply Preset");
    options.insert(options.end(), "Save");
    options.insert(options.end(), "Save As");
    options.insert(options.end(), "Return");

    int selection = -1;
    do {
        selection = menu(options, "Editor:");
        switch (selection) {
            case 0:
                filterMenu();
                break;
            case 1:
                presetMenu();
                break;
            case 2:
                saveMenu(false);
                break;
            case 3:
                saveMenu(true);
                break;
            case 4:
                return;
                break;
        }
        selection = -1;
    } while (selection == -1);
}

void saveMenu(bool enterName) {
    clearScreen();
    cout << "Save:\n\n";
    if (saveName == "" || enterName) {
        cout << "Enter name: ";
        cin >> saveName;
    }

    (*image).write(saveName + ".bmp");
}

void presetMenu() {
    clearScreen();
    string path;
    FILE *file;
    cout << "Load Preset: \n\n";
    do {
        cout << "Enter the path to the preset or (cancel): \n";
        cin >> path;
        file = fopen(path.c_str(), "r");
    } while (!file && path != "cancel");

    cout << "Processing...\n";
    char line[256];
    while (fgets(line, 256, file)) {
        string preset(line);
        if (startsWith(preset, "colorNegative")) {
            colorNegativeFilter(true);
        } else if (startsWith(preset, "grayscale")) {
            float intensity = getPresetFloat(preset);
            grayscale(true, intensity);
        } else if (startsWith(preset, "sepiaTone")) {
            float intensity = getPresetFloat(preset);
            sepiaTone(true, intensity);
        } else if (startsWith(preset, "colorFilter")) {
            int index = preset.find(':');
            stringstream ss(preset.substr(index + 1));
            vector<float> values;
            while (ss.good()) {
                string substr;
                getline(ss, substr, ',');
                if (substr != "") {
                    values.push_back(stof(substr));
                }
            }
            float r = 1;
            float g = 1;
            float b = 1;
            if (values.size() == 3) {
                r = values[0];
                g = values[1];
                b = values[2];
            }
            colorFilter(true, r, g, b);
        } else if (startsWith(preset, "averageBlur")) {
            int radius = getPresetInt(preset);
            averageBlur(true, radius);
        } else if (startsWith(preset, "gaussianBlur")) {
            int radius = getPresetInt(preset);
            gaussianBlur(true, radius);
        } else if (startsWith(preset, "edgeDetection")) {
            edgeDetection(true);
        } else if (startsWith(preset, "chromaticAberration")) {
            int displacement = getPresetInt(preset);
            chromaticAberration(true, displacement);
        } else if (startsWith(preset, "brightness")) {
            int intensity = getPresetInt(preset);
            brightness(true, intensity);
        } else if (startsWith(preset, "contrast")) {
            int intensity = getPresetInt(preset);
            contrast(true, intensity);
        } else if (startsWith(preset, "gamma")) {
            float value = getPresetFloat(preset);
            gammaCorrection(true, value);
        } else if (startsWith(preset, "scanLines")) {
            float intensity = getPresetFloat(preset);
            scanlines(true, intensity);
        } else if (startsWith(preset, "fishEye")) {
            fishEye(true);
        }else if(startsWith(preset, "pixelate")){
            int size = getPresetInt(preset);
            pixelate(true, size);
        }
    }
    fclose(file);
    cout << "Done!\n";
    system("pause");
}

void filterMenu() {
    vector<string> filters;
    filters.push_back("Cancel");
    filters.push_back("Color Negative");
    filters.push_back("Grayscale");
    filters.push_back("Sepia Tone");
    filters.push_back("Color Filterring");
    filters.push_back("Average Blur");
    filters.push_back("Gaussian Blur");
    filters.push_back("Edge Detection");
    filters.push_back("Chromatic Aberration");
    filters.push_back("Brightness");
    filters.push_back("Contrast");
    filters.push_back("Gamma Correction");
    filters.push_back("Scan lines");
    filters.push_back("Fish Eye");
    filters.push_back("Pixelate");
    int selection = -1;
    do {
        selection = menu(filters, "Filters: ");
        clearScreen();
        switch (selection) {
            case 0:
                return;
                break;
            case 1:
                colorNegativeFilter(false);
                break;
            case 2:
                grayscale(false, 0);
                break;
            case 3:
                sepiaTone(false, 0);
                break;
            case 4:
                colorFilter(false, 1, 1, 1);
                break;
            case 5:
                averageBlur(false, 0);
                break;
            case 6:
                gaussianBlur(false, 0);
                break;
            case 7:
                edgeDetection(false);
                break;
            case 8:
                chromaticAberration(false, 0);
                break;
            case 9:
                brightness(false, 0);
                break;
            case 10:
                contrast(false, 0);
                break;
            case 11:
                gammaCorrection(false, 0);
                break;
            case 12:
                scanlines(false, 0);
                break;
            case 13:
                fishEye(false);
                break;
            case 14:
                pixelate(false, 0);
                break;
        }
    } while (selection == -1);
}

void colorNegativeFilter(bool fromPreset) {
    if (!fromPreset) {
        cout << "Color Negative:\n\n";
        cout << "Processing...\n";
    }

    NegativeFilter filter;
    filter.apply(image);

    if (!fromPreset) {
        cout << "Filter applied!\n";
        system("pause");
    }
}

void grayscale(bool fromPreset, float intensity) {
    if (!fromPreset) {
        cout << "Grayscale:\n\n";
        cout << "Enter filter intensity (0.0 - 1.0): ";
        cin >> intensity;
        cout << "Processing...\n";
    }

    GrayscaleFilter filter(intensity);
    filter.apply(image);

    if (!fromPreset) {
        cout << "Filter applied!\n";
        system("pause");
    }
}

void sepiaTone(bool fromPreset, float intensity) {
    if (!fromPreset) {
        cout << "Sepia Tone:\n\n";
        cout << "Enter filter intensity (0.0 - 1.0): ";
        cin >> intensity;
        cout << "Processing...\n";
    }
    SepiaToneFilter filter(intensity);
    filter.apply(image);
    if (!fromPreset) {
        cout << "Filter applied!\n";
        system("pause");
    }
}

void colorFilter(bool fromPreset, float r, float g, float b) {
    if (!fromPreset) {
        cout << "Color Filter:\n\n";
        cout << "Enter the amount of red (0.0 - 1.0): ";
        cin >> r;
        cout << "Enter the amount of green (0.0 - 1.0): ";
        cin >> g;
        cout << "Enter the amount of blue (0.0 - 1.0): ";
        cin >> b;
        cout << "Processing...\n";
    }
    ColorFilter filter(r, g, b);
    filter.apply(image);
    if (!fromPreset) {
        cout << "Filter applied!\n";
        system("pause");
    }
}

void averageBlur(bool fromPreset, int radius) {
    if (!fromPreset) {
        cout << "Average Blue:\n\n";
        cout << "Enter the radius of the blur: ";
        cin >> radius;
        cout << "Processing...\n";
    }
    AverageBlurFilter filter(radius);
    filter.apply(image);
    if (!fromPreset) {
        cout << "Filter applied!\n";
        system("pause");
    }
}

void gaussianBlur(bool fromPreset, int radius) {
    if (!fromPreset) {
        cout << "Gaussian Blur:\n\n";
        cout << "Enter the radius of the blur: ";
        cin >> radius;
        cout << "Processing...\n";
    }
    GaussianBlurFilter filter(radius);
    filter.apply(image);
    if (!fromPreset) {
        cout << "Filter applied!\n";
        system("pause");
    }
}

void edgeDetection(bool fromPreset) {
    if (!fromPreset) {
        cout << "Edge Detection:\n\n";
        cout << "Processing...\n";
    }

    EdgeDetectionFilter filter;
    filter.apply(image);
    if (!fromPreset) {
        cout << "Filter applied!\n";
        system("pause");
    }
}

void chromaticAberration(bool fromPreset, int displacement) {
    if (!fromPreset) {
        cout << "Chromatic Aberration:\n\n";
        cout << "Enter the amount of displacement: ";
        cin >> displacement;
        cout << "Processing...\n";
    }

    ChromaticAberrationFilter filter(displacement);
    filter.apply(image);
    if (!fromPreset) {
        cout << "Filter applied!\n";
        system("pause");
    }
}

void brightness(bool fromPreset, int intensity) {
    if (!fromPreset) {
        cout << "Brightness: \n\n";
        cout << "Enter brightness: ";
        cin >> intensity;
        cout << "Processing...\n";
    }

    BrightnessFilter filter(intensity);
    filter.apply(image);
    if (!fromPreset) {
        cout << "Filter applied!\n";
        system("pause");
    }
}

void contrast(bool fromPreset, int intensity) {
    if (!fromPreset) {
        cout << "Contrast: \n\n";
        cout << "Enter contrast (-255 - 255): ";
        cin >> intensity;
        cout << "Processing...\n";
    }

    ContrastFilter filter(intensity);
    filter.apply(image);
    if (!fromPreset) {
        cout << "Filter applied!\n";
        system("pause");
    }
}

void gammaCorrection(bool fromPreset, float value) {
    if (!fromPreset) {
        cout << "Gamma Correction: \n\n";
        cout << "Enter gamma (0.01 - 10.0): ";
        cin >> value;
        cout << "Processing...\n";
    }

    GammaCorrectionFilter filter(value);
    filter.apply(image);
    if (!fromPreset) {
        cout << "Filter applied!\n";
        system("pause");
    }
}

void scanlines(bool fromPreset, float intensity) {
    if (!fromPreset) {
        cout << "Scan lines: \n\n";
        cout << "Enter scanline intensity (0.0 - 1.0): ";
        cin >> intensity;
        cout << "Processing...\n";
    }
    ScanlineFilter filter(intensity);
    filter.apply(image);
    if (!fromPreset) {
        cout << "Filter applied!\n";
        system("pause");
    }
}

void fishEye(bool fromPreset) {
    if (!fromPreset) {
        cout << "Fish Eye:\n\n";
        cout << "Processing...\n";
    }

    FishEyeFilter filter;
    filter.apply(image);
    if (!fromPreset) {
        cout << "Filter applied!\n";
        system("pause");
    }
}

void pixelate(bool fromPreset, int size){
    if(!fromPreset){
        cout << "Pixelate:\n\n";
        cout << "Enter the size of the pixel: ";
        cin >> size;
        cout << "Processing...\n";
    }
    PixelateFilter filter(size);
    filter.apply(image);
    if(!fromPreset){
        cout << "Filter applied!\n";
        system("pause");
    }
}

void quit(int code) {
    cout << "Goodbye!\n";
    exit(code);
}