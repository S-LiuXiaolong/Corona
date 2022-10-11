#include <iostream>
#include "AssetLoader.h"
#include "MemoryManager.h"

#include "lodepng/lodepng.h"

/*
3 ways to decode a PNG from a file to RGBA pixel data (and 2 in-memory ways).
*/

//g++ lodepng.cpp example_decode.cpp -ansi -pedantic -Wall -Wextra -O3


//Example 1
//Decode from disk to raw pixels with a single function call
void decodeOneStep(const char* filename) {
    std::vector<unsigned char> image; //the raw pixels
    unsigned width, height;

    //decode
    unsigned error = lodepng::decode(image, width, height, filename);

    //if there's an error, display it
    if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

    //the pixels are now in the vector "image", 4 bytes per pixel, ordered RGBARGBA..., use it as texture, draw it, ...
}

//Example 2
//Load PNG file from disk to memory first, then decode to raw pixels in memory.
void decodeTwoSteps(const char* filename) {
    std::vector<unsigned char> png;
    std::vector<unsigned char> image; //the raw pixels
    unsigned width, height;

    //load and decode
    unsigned error = lodepng::load_file(png, filename);
    if(!error) error = lodepng::decode(image, width, height, png);

    //if there's an error, display it
    if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

    //the pixels are now in the vector "image", 4 bytes per pixel, ordered RGBARGBA..., use it as texture, draw it, ...
}

//Example 3
//Load PNG file from disk using a State, normally needed for more advanced usage.
void decodeWithState(const char* filename) {
    std::vector<unsigned char> png;
    std::vector<unsigned char> image; //the raw pixels
    unsigned width, height;
    lodepng::State state; //optionally customize this one

    unsigned error = lodepng::load_file(png, filename); //load the image file with given filename
    if(!error) error = lodepng::decode(image, width, height, state, png);

    //if there's an error, display it
    if(error) std::cout << "decoder error " << error << ": "<< lodepng_error_text(error) << std::endl;

    //the pixels are now in the vector "image", 4 bytes per pixel, ordered RGBARGBA..., use it as texture, draw it, ...
    //State state contains extra information about the PNG such as text chunks, ...
}

using namespace Corona;

namespace Corona {
    MemoryManager* g_pMemoryManager = new MemoryManager();
    AssetLoader*   g_pAssetLoader = new AssetLoader();
}

int main(int argc, char *argv[]) 
{
    g_pMemoryManager->Initialize();
    g_pAssetLoader->Initialize();

#ifdef __ORBIS__
    g_pAssetLoader->AddSearchPath("/app0");
#endif

    {
        std::string path;
        if (argc >= 2) {
            path = g_pAssetLoader->GetFilePath(argv[1]);
        } else {
            path = g_pAssetLoader->GetFilePath("Textures/test.png");
        }

        decodeWithState(path.c_str());
    }
}
