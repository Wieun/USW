#define _USE_MATH_DEFINES
#include <tbb/parallel_for.h>
#include <tbb/parallel_reduce.h>
#include <tbb/blocked_range2d.h>
#include <random>
#include <chrono>
#include <iostream>
#include <vector>
//Thread building blocks library
#include <tbb/task_scheduler_init.h>
//Free Image library
#include <FreeImagePlus.h>

using namespace std::chrono;
using namespace std;
using namespace tbb;

int main()
{
    int nt = task_scheduler_init::default_num_threads();
    task_scheduler_init T(nt);

    //Part 1 (Greyscale Gaussian blur): -----------DO NOT REMOVE THIS COMMENT----------------------------//

    fipImage gaussianBlurInputImage;
    gaussianBlurInputImage.load("../Images/image.jpg");
    gaussianBlurInputImage.convertToFloat();

    unsigned int width = gaussianBlurInputImage.getWidth();
    unsigned int height = gaussianBlurInputImage.getHeight();

    fipImage gaussianBlurOutputImage;
    gaussianBlurOutputImage = fipImage(FIT_FLOAT, width, height, 24);

    auto *inputBuffer = (float*)gaussianBlurInputImage.accessPixels();
    auto *outputBuffer = (float*)gaussianBlurOutputImage.accessPixels();

    //////////////////////////////////////////////////
    const int kernelSize = 11;
    float sigma = 3.0f;
    double kernel[kernelSize][kernelSize];
    int kernelRadius = kernelSize/ 2; //The radius of the input image
    double sumTotal = 0;

    ///////////////////////////////////////////////////////

    cout << "<<<<<<<<<Kernel>>>>>>>>>\n";
    auto sequentialStart = high_resolution_clock::now();

    //create gaussian filter
    for (int y = -kernelRadius; y <= kernelRadius; ++y)
    {
        for (int x = -kernelRadius; x <= kernelRadius; ++x){

            kernel[y + kernelRadius][x + kernelRadius] = 1.0f / (2.0f * float(M_PI)) * powf(sigma,2) * exp(-((powf(x,2) + powf(y,2)) / (2.0f * powf(sigma,2))));
            sumTotal += kernel[y + kernelRadius][x + kernelRadius];
        }
    }

    //normalise the kernel
    for (int x = 0; x < kernelSize; x++){

        for (int y = 0; y < kernelSize; y++){

            kernel[y][x] = kernel[y][x] * (1.0 / sumTotal);
            cout << kernel[y][x] << "\t";
        }cout << endl;
    }

    //create the input Image kernel
    for (int i = kernelRadius; i < height - kernelRadius; i++) {

        for (int j = kernelRadius; j < width - kernelRadius; j++) {

            for (int x = -kernelRadius; x <= kernelRadius; x++) {

                for (int y = -kernelRadius; y <= kernelRadius; y++) {

                    outputBuffer[i * width + j] += inputBuffer[(i + x) * width + (j + y)] * kernel[x + kernelRadius][y + kernelRadius];
                }
            }
        }
    }

    auto sequentialFinish = high_resolution_clock::now();

    auto sequentialDuration = duration_cast<milliseconds>(sequentialFinish - sequentialStart);
    std::cout << "Sequential duration: " << sequentialDuration.count() << " ms." << endl;

    ///////////////////////////////////////////////////////

    auto parallelForStart = high_resolution_clock::now();

    parallel_for(blocked_range2d<int,int>(kernelRadius, height - kernelRadius,1000, kernelRadius, height - kernelRadius,1000), [&](const blocked_range2d<int,int>& range) {

        auto y1 = range.rows().begin();
        auto y2 = range.rows().end();
        auto x1 = range.cols().begin();
        auto x2 = range.cols().end();

        for (int i = x1; i < x2; i++) {

            for (int j = y1; j < y2; j++) {

                for (auto x = -kernelRadius; x <= kernelRadius; x++) {

                    for (auto y = -kernelRadius; y <= kernelRadius; y++) {

                        outputBuffer[i * width + j] += inputBuffer[(i + x) * width + (j + y)] * kernel[x + kernelRadius][y + kernelRadius];
                    }
                }
            }
        }
    });

    auto parallelForFinish = high_resolution_clock::now();

    auto parallelForDuration = duration_cast<milliseconds>(parallelForFinish - parallelForStart);
    std::cout << "Parallel for duration: " << parallelForDuration.count() << " ms." << endl;

    //////////////////////////////////////////////////

    gaussianBlurOutputImage.convertToType(FREE_IMAGE_TYPE::FIT_BITMAP);
    gaussianBlurOutputImage.convertTo24Bits();
    gaussianBlurOutputImage.save("grey_blurred.png");
    cout << "Gaussian blur image saved! " << endl;

    //Part 2 (Colour image processing): -----------DO NOT REMOVE THIS COMMENT----------------------------//

    ////////////////////////////////ABSOLUTE DIFFERENCE BETWEEN 2 IMAGES//////////////////////////////
    // Setup Input image array

    fipImage inputImage[2];
    inputImage[0].load("../Images/render_1.png");
    inputImage[1].load("../Images/render_2.png");

    width = inputImage[0].getWidth();
    height = inputImage[0].getHeight();

    // Setup Output image array
    fipImage outputImage;
    outputImage = fipImage(FIT_BITMAP, width, height, 24);

    //2D Vector to hold the RGB colour data of an image
    vector<vector<RGBQUAD>> rgbValues;
    rgbValues.resize(height, vector<RGBQUAD>(width));

    // parallel_for blocked_range version
    parallel_for(blocked_range2d <int> (0,height,0,width),[&](const blocked_range2d <int>& range) {

        auto y1 = range.rows().begin();
        auto y2 = range.rows().end();
        auto x1 = range.cols().begin();
        auto x2 = range.cols().end();

        for (uint64_t y = y1; y != y2; y++) {

            for (uint64_t x = x1; x != x2; x++) {

                RGBQUAD rgb[2];  //FreeImage structure to hold RGB values of a single pixel

                inputImage[0].getPixelColor(x,y,&rgb[0]);
                inputImage[1].getPixelColor(x,y,&rgb[1]);

                if (abs(rgb[0].rgbRed - rgb[1].rgbRed) > 1 ||
                    abs(rgb[0].rgbGreen - rgb[1].rgbGreen) > 1 ||
                    abs(rgb[0].rgbBlue - rgb[1].rgbBlue) > 1 ) {
                        rgbValues[y][x].rgbRed = 255;
                        rgbValues[y][x].rgbGreen = 255;
                        rgbValues[y][x].rgbBlue = 255;
                }
                outputImage.setPixelColor(x, y, &rgbValues[y][x]);
            }
        }
    });

    //////////////////////////////////////////CALCULATE THE PERCENTAGE OF WHITE PIXELS////////////////////////////////

    float totalNoOfPixels = height * width;

    int noOfWhitePixels = parallel_reduce(blocked_range2d <int> (0,height,0,width),0,[&](const blocked_range2d <int>& range, int count) -> int{

        auto y1 = range.rows().begin();
        auto y2 = range.rows().end();
        auto x1 = range.cols().begin();
        auto x2 = range.cols().end();

        for (int y = y1; y != y2; y++)

            for (int x = x1; x != x2; x++) {

                if (rgbValues[y][x].rgbRed > 1) count++;

            }return count;

        }, [&](int x, int y)->int {return x+y;}

    );

    float percentageOfWhitePixels = (noOfWhitePixels / totalNoOfPixels) * 100;

    cout << "There is " << percentageOfWhitePixels << "% of white pixels." << endl;

    //////////////////////////////////////////SET AND FIND RED DOT//////////////////////////////////////

    random_device rd; // obtain a random number from hardware
    mt19937 eng(rd()); // seed the generator
    uniform_int_distribution<> yAxis(0, height); // define the range
    uniform_int_distribution<> xAxis(0, width); // define the range

    rgbValues[yAxis(eng)][xAxis(eng)].rgbRed = 255;
    rgbValues[yAxis(eng)][xAxis(eng)].rgbGreen = 0;
    rgbValues[yAxis(eng)][xAxis(eng)].rgbBlue = 0;

    outputImage.setPixelColor(xAxis(eng), yAxis(eng), &rgbValues[yAxis(eng)][xAxis(eng)]);

    auto t2b = high_resolution_clock::now();
    int returnIndex[2];

    parallel_for(blocked_range2d <int> (0,height,0,width),[&](const blocked_range2d <int>& range) {

        auto y1 = range.rows().begin();
        auto y2 = range.rows().end();
        auto x1 = range.cols().begin();
        auto x2 = range.cols().end();

        for (int y = y1; y != y2; y++) {

            for (int x = x1; x != x2; x++) {

                if (rgbValues[y][x].rgbRed == 255 &&
                    rgbValues[y][x].rgbGreen == 0 &&
                    rgbValues[y][x].rgbBlue == 0){

                    if(task::self().cancel_group_execution()){
                        returnIndex[0] = x;
                        returnIndex[1] = y;
                    }
                }
            }
        }
    });

    auto t2e = high_resolution_clock::now();
    auto t2r = duration_cast<milliseconds>(t2e - t2b);

    cout << t2r.count() << endl;

    cout << "The red pixel has been located at x = " << returnIndex[0] << " and y = " << returnIndex[1] << endl;
    //Save the processed image
    outputImage.save("RGB_processed.png");

    return 0;
}