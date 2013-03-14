#pragma once

namespace nui{
    
    struct AudioConfig{
        AudioConfig(unsigned int samplingRate = 44100, unsigned int channels = 2, unsigned int frequency = 16)
            : samplingRate(samplingRate) , channels(channels) , frequency(frequency)
        { }
        unsigned int samplingRate;
        unsigned int channels;
        unsigned int frequency;
    };

    class AudioInputDevice{
    public:
        virtual void setAudioConfig(const AudioConfig & config) = 0;
        virtual AudioConfig audioConfig() = 0;
        virtual const unsigned char* audioBuffer() = 0;
        virtual ~AudioInputDevice(){ }

    };

} //namespace nui
