#pragma once

#include <cstdint>
#include <iostream>

#ifdef OPENCV_SUPPORT
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#else
#ifdef CCIMG_SUPPORT
#include "CImg.h"
#endif
#endif

#include "GenTL/PFNC.h"

#ifdef OPENCV_SUPPORT
class Image_displayer {
    public:
        void convert(const uint8_t *pRawBuffer, int width, int height, int type);
        void caleo_convert(const uint8_t *pRawBuffer, size_t pOutRows, size_t pOutColumns);
        void resize_and_display(std::string windowName);
    private:
        cv::Mat converted;
        cv::Mat outputFrame;
};
#else
#ifdef CIMG_SUPPORT
class Image_displayer {
    public:
        void convert(const uint8_t *pRawBuffer, int width, int height, int type);
        void caleo_convert(const uint8_t *pRawBuffer, size_t pOutRows, size_t pOutColumns);
        void resize_and_display(std::string windowName);
    private:
        cimg_library::CImg<unsigned char> converted;
        cimg_library::CImgDisplay main_disp;
};
#else
class Image_displayer {
public:
    void convert(const uint8_t *pRawBuffer, int width, int height, int type);
    void caleo_convert(const uint8_t *pRawBuffer, size_t pOutRows, size_t pOutColumns);
    void resize_and_display(std::string windowName);
};
#endif
#endif
