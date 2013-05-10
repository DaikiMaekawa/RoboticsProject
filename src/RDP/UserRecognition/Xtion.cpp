#include "Xtion.h"
#include "XnErrorChecker.h"

namespace nui{

    bool Xtion::isValid(){
        std::cout << m_audioGenerator.IsValid() << std::endl;
        return m_imageGenerator.IsValid() && m_depthGenerator.IsValid() && m_audioGenerator.IsValid();
    }
    
    Xtion::Xtion(xn::Context &context){
        try{
            initImageGenerator(context);
	        initDepthGenerator(context);
            //initAudioGenerator(context);
        }catch(std::exception &ex){
            std::cout << ex.what() << std::endl;
        }
    }

    void Xtion::initDepthGenerator(const xn::Context &context){
        xnErrorCheck(context.FindExistingNode(XN_NODE_TYPE_DEPTH,m_depthGenerator));
	    xnErrorCheck(m_depthGenerator.GetAlternativeViewPointCap().SetViewPoint(m_imageGenerator));
    }

    void Xtion::initImageGenerator(const xn::Context &context){
	    xnErrorCheck(context.FindExistingNode(XN_NODE_TYPE_IMAGE,m_imageGenerator));
	    //m_imageGenerator.GetAlternativeViewPointCap().SetViewPoint(m_depthGenerator);
    }
    
    void Xtion::initAudioGenerator(xn::Context &context){
        //xnErrorCheck(context.FindExistingNode(XN_NODE_TYPE_AUDIO,m_imageGenerator));
        xnErrorCheck(m_audioGenerator.Create(context));
        AudioConfig config;
        setAudioConfig(config);
    }

    NIMat Xtion::rgbImage(){
        XnMapOutputMode imageMap;
        m_imageGenerator.GetMapOutputMode(imageMap);

        xn::ImageMetaData metaData;
        m_imageGenerator.GetMetaData(metaData);
        
        NIMat image(new cv::Mat(imageMap.nYRes,imageMap.nXRes,CV_8UC3));

        xnOSMemCopy(image->data,metaData.RGB24Data(),imageMap.nXRes * imageMap.nYRes * 3);
	    cv::cvtColor(*image, *image, CV_RGB2BGR);

        return image;
    }


    NIMat Xtion::depthImage(){
        
        XnMapOutputMode depthMap;
        m_depthGenerator.GetMapOutputMode(depthMap);
        
        const int numPixels = depthMap.nXRes * depthMap.nYRes;
        unsigned char *grayPixels = new unsigned char[numPixels];

        xnOSMemSet(grayPixels,0,numPixels * sizeof(unsigned char));

        xn::DepthMetaData metaData;
        m_depthGenerator.GetMetaData(metaData);
        const XnDepthPixel *depthPixels = metaData.Data();

        for(int i=0; i < numPixels; i++)
            grayPixels[i] = (int)(std::min(10000, (int)depthPixels[i]) / (10000/255));
        
        NIMat depth(new cv::Mat(depthMap.nYRes,depthMap.nXRes,CV_8UC1));
        xnOSMemCopy(depth->data,grayPixels,numPixels);
        delete[] grayPixels;
        
        return depth;
    }

    void Xtion::convertRealWorldToProjective(float &x, float &y, float &z){
        XnPoint3D real[1];
        XnPoint3D projective[1];
        real[0].X = x;
        real[0].Y = y;
        real[0].Z = z;
        
        m_depthGenerator.ConvertRealWorldToProjective(1,real,projective);

        x = projective[0].X;
        y = projective[0].Y;
        z = projective[0].Z;
    }

    void Xtion::setAudioConfig(const nui::AudioConfig &config){
        XnWaveOutputMode mode;
        mode.nSampleRate = config.samplingRate;
        mode.nChannels = config.channels;
        mode.nBitsPerSample = config.frequency;
        xnErrorCheck(m_audioGenerator.SetWaveOutputMode(mode));
    }

    AudioConfig Xtion::audioConfig(){
        XnWaveOutputMode mode;
        m_audioGenerator.GetWaveOutputMode(mode);
        AudioConfig config(mode.nSampleRate , mode.nChannels , mode.nBitsPerSample);
        return config;
    }

} //namespace nui
