#include "Image.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>
#include <exception>

std::string CHARS = "@%#+?*;=-:.` ";
int CHARS_LENGTH = CHARS.length();
int INTERVAL = 256 / CHARS_LENGTH;

char getChar(int gray)
{
    std::reverse(CHARS.begin(), CHARS.end());
    int idx = gray / INTERVAL;
    if (idx >= CHARS_LENGTH) idx = CHARS_LENGTH - 1;
    return CHARS[idx];
}

const float SCALE_FACTOR = 0.2;
const float ONE_CHAR_RATIO = 8.0f / 18.0f;


int main()
{
    try
    {
        std::cout << "Working dir: " << std::filesystem::current_path().string() << "\n";

        Image test("Pictures/pepe.jpg");

        std::cout << "Original size: " << test.width << "x" << test.height << "\n";

        float widthScale = SCALE_FACTOR; 
        float heightScale = SCALE_FACTOR * ONE_CHAR_RATIO;
        test.resize(widthScale, heightScale);

        std::cout << "Resize: " << test.width << "x" << test.height << "\n";

        //choose between luminance grayscale or average grayscale 

        Image grayAvg = test;
        grayAvg.grayscaleAvg().write("grayAvg2.png");

        Image grayLum = test;
        grayLum.grayscaleLum().write("grayLum2.png");

        std::ofstream outFile("output.txt");
        if (!outFile.is_open()) {
            std::cerr << "Failed to create output.txt" << std::endl;
            return 1;
        }

        for (int y = 0; y < grayLum.height; ++y)
        {
            for (int x = 0; x < grayLum.width; ++x)
            {
                int gray = grayLum.getGray(x, y, 2); // using lum version for gray
                char writeToFile = getChar(gray);
                outFile << writeToFile;
            }
            outFile << '\n';
        }
        outFile.close();  
        std::cout << "ASCII art saved to: " << std::filesystem::absolute("output.txt") << std::endl;

    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}