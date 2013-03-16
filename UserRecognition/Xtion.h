#pragma once

#include "ImageDevice.h"
#include "AudioDevice.h"
#include <XnCppWrapper.h>

namespace nui{

    class Xtion : public AudioInputDevice , public ImageDevice
    {
        static const XnUInt16 MAX_DEPTH_NUM = 10000;
        xn::ImageGenerator m_imageGenerator;
        xn::DepthGenerator m_depthGenerator;
        xn::AudioGenerator m_audioGenerator;
        void initDepthGenerator(const xn::Context &context);
        void initImageGenerator(const xn::Context &context);
        void initAudioGenerator(xn::Context &context);

    public:
        bool isValid();
        NIMat rgbImage();
        NIMat depthImage();
        void convertRealWorldToProjective(float &x,float &y, float &z);
        Xtion(xn::Context &context);
        void setAudioConfig(const AudioConfig &config);
        AudioConfig audioConfig();
        const unsigned char* audioBuffer(){ return m_audioGenerator.GetAudioBuffer(); }
    };

} //namespace nui
