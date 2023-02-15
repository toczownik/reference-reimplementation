#include "Image_displayer.h"

#ifdef OPENCV_SUPPORT
void Image_displayer::convert(const uint8_t *dataPointer, int width, int height, int pixelFormat) {
    if(pixelFormat == PFNC_Mono8){
        cv::Mat depthFrame((int) height, (int) width,CV_8UC1, (void *)dataPointer);
        depthFrame.convertTo(converted, CV_8UC1, 255.0 / (pow(2, 8) - 1));
    }
    else if (pixelFormat == PFNC_Mono10 || pixelFormat == PFNC_Mono12 || pixelFormat == PFNC_Mono14 ||
             pixelFormat == PFNC_Mono16) {
        cv::Mat depthFrame((int) height, (int) width,CV_16UC1, (void *)dataPointer);
        if (pixelFormat == PFNC_Mono10) {
            depthFrame.convertTo(converted, CV_8UC1, 255.0 / (pow(2, 10) - 1));
        } else if (pixelFormat == PFNC_Mono12) {
            depthFrame.convertTo(converted, CV_8UC1, 255.0 / (pow(2, 12) - 1));
        } else if (pixelFormat == PFNC_Mono14) {
            depthFrame.convertTo(converted, CV_8UC1, 255.0 / (pow(2, 14) - 1));
        } else if (pixelFormat == PFNC_Mono16) {
            depthFrame.convertTo(converted, CV_8UC1, 255.0 / (pow(2, 16) - 1));
        }
    }
}

void Image_displayer::caleo_convert(const uint8_t *dataPointer, size_t pOutRows, size_t pOutColumns) {
    size_t rawColumns;
    rawColumns = floor((double)pOutColumns/6)*12+9;
    cv::Mat depthFrame( (int) pOutRows,(int) pOutColumns, CV_8UC1);
    for (int y = 0; y < depthFrame.rows; y++) {
        for (int x = 0; x < depthFrame.cols; x++) {
            depthFrame.at<uint8_t>(y,x) =((*(dataPointer + 2*x +1 + rawColumns*y)));
        }
    }
    converted = depthFrame;
}

void Image_displayer::resize_and_display(std::string windowName) {
    double resizeRatio = 1024.0 / (double) converted.cols;
    if (resizeRatio > 768.0 / (double) converted.rows) {
        resizeRatio = 768.0 / (double) converted.rows;
    }
    cv::resize(converted, outputFrame, cv::Size(), resizeRatio, resizeRatio);
    cv::imshow(std::string("Image from ") + std::string(windowName), outputFrame);
    cv::waitKey(10);
}


#else
#ifdef CIMG_SUPPORT
void Image_displayer::convert(const uint8_t *dataPointer, int width, int height, int pixelFormat) {
    if (pixelFormat == PFNC_Mono8) {
        cimg_library::CImg<unsigned char> img(dataPointer, width, height, 1, 1, false);
        converted = img;
    } else if (pixelFormat == PFNC_Mono10 || pixelFormat == PFNC_Mono12 || pixelFormat == PFNC_Mono14 ||
               pixelFormat == PFNC_Mono16) {
        cimg_library::CImg<uint16_t> depthFrame((int) height, (int) width);
        for (int x = 0; x < height * width; x++) {
            if (pixelFormat == PFNC_Mono10) {
                converted.at(x) = ((((uint16_t *) dataPointer)[x])) >> 2;
            } else if (pixelFormat == PFNC_Mono12) {
                converted.at(x) = ((((uint16_t *) dataPointer)[x])) >> 4;
            } else if (pixelFormat == PFNC_Mono14) {
                converted.at(x) = ((((uint16_t *) dataPointer)[x])) >> 6;
            } else if (pixelFormat == PFNC_Mono16) {
                converted.at(x) = ((((uint16_t *) dataPointer)[x])) >> 8;
            }
        }
    }
}

void Image_displayer::caleo_convert(const uint8_t *dataPointer, size_t pOutRows, size_t pOutColumns) {
    size_t rawColumns;
    rawColumns = floor((double)pOutColumns/6)*12+9;
    cimg_library::CImg<unsigned char> depthFrame( (int) pOutRows,(int) pOutColumns);
    for (int y = 0; y < depthFrame.height(); y++) {
        for (int x = 0; x < depthFrame.width(); x++) {
            depthFrame.at(x,y) =((*(dataPointer + 2*x +1 + rawColumns*y)));
        }
    }
    converted = depthFrame;
}

void Image_displayer::resize_and_display(std::string windowName) {
            double resizeRatio = 512.0 / (double) converted.width();
            if (resizeRatio > 378.0 / (double) converted.height()) {
                resizeRatio = 378.0 / (double) converted.height();
            }
            converted.resize((-100) * resizeRatio, (-100) * resizeRatio, -100, -100, 3);
            if(main_disp.width() != converted.width() || main_disp.height() != converted.height()){
                main_disp.resize(converted);
            }
            main_disp.set_title(windowName.c_str());
            main_disp.display((converted));
}

#else
void Image_displayer::convert(const uint8_t *dataPointer, int width, int height, int pixelFormat) {

}

void Image_displayer::caleo_convert(const uint8_t *dataPointer, size_t pOutRows, size_t pOutColumns) {

}

void Image_displayer::resize_and_display(std::string windowName) {

}
#endif
#endif