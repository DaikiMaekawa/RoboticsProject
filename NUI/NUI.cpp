#include "NUI.h"
#include <XnCppWrapper.h>
#include "UserDetector.h"
#include "Xtion.h"
#include "XnErrorChecker.h"

namespace nui{

    class NUI::Impl{
        xn::Context m_context;
        boost::shared_ptr<Xtion>           m_device;
        boost::shared_ptr<UserDetector>    m_userDetector;

    public:
        Impl(){
            try{
                //xnErrorCheck(m_context.InitFromXmlFile("../SamplesConfig.xml"));
                xnErrorCheck(m_context.InitFromXmlFile("../InitDevice.xml"));
                xnErrorCheck(xnPrintRegisteredLicenses());
                m_context.SetGlobalMirror(true);
                
                m_device = boost::shared_ptr<Xtion>(new Xtion(m_context));
                m_userDetector = boost::shared_ptr<UserDetector>(new UserDetector(*m_device,m_context));
            }catch(std::exception &ex){
                std::cout << ex.what() << std::endl;
            }
        }
    	
        void startProcessing(){
            try{
                xnErrorCheck(m_context.StartGeneratingAll());
            }catch(std::exception &ex){
                std::cout << ex.what() << std::endl;
            }
        }

        void waitUpdateAll(){
            m_context.WaitAndUpdateAll();
            //std::cout << m_device->audioBuffer() << std::endl;
        }

        NIMat depthImage(){
            return m_device->depthImage();
        }
        
        NIMat rgbImage(){
            return m_device->rgbImage();
        }

        NIMat userDepthImage(const UserStatus &user){
            return m_userDetector->userDepthMetaData(user);
        }

        const std::vector<UserStatus>& detectUsers(){
            m_userDetector->updateAllUserStatus();
            return m_userDetector->detectUsers();
        }
    	
        bool isValid() const{
            return m_userDetector->isValid() && m_device->isValid();
        }

        ~Impl(){
            m_context.Shutdown();
        }

    };

    NUI::NUI() : pImpl(new Impl())
    {

    }

    void NUI::startProcessing(){
        return pImpl->startProcessing();
    }

    void NUI::waitUpdateAll(){
        return pImpl->waitUpdateAll();
    }	

    NIMat NUI::depthImage(){
        return pImpl->depthImage();
    }

    NIMat NUI::rgbImage(){
        return pImpl->rgbImage();
    }

    NIMat NUI::userDepthImage(const UserStatus &user){
        return pImpl->userDepthImage(user);
    }

    const std::vector<UserStatus>& NUI::detectUsers(){
        return pImpl->detectUsers();
    }

    bool NUI::isValid() const{
        return pImpl->isValid();
    }

} //namespace nui
