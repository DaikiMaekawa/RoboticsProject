#pragma once
#include <opencv2/opencv.hpp>
#include <boost/shared_ptr.hpp>

namespace nui{
    typedef boost::shared_ptr<cv::Mat> NIMat;

    class ImageDevice{
    public:
        virtual bool isValid() = 0;
        virtual NIMat depthImage() = 0;
        virtual NIMat rgbImage() = 0;
        virtual void convertRealWorldToProjective(float &x,float &y, float &z) = 0;
        virtual ~ImageDevice(){ }
    };

} //namespace nui
