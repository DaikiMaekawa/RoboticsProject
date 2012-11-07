#pragma once

#include <SpeechRecognition.h>
#include <UserRecognition.h>
#include <Atlas/Timer.h>
#include <UserStatus.h>
#include <boost/shared_ptr.hpp>

class Unity{
    nui::SpeechRecognition m_sRecog;
    nui::UserRecognition   m_uRecog;
    atlas::Timer m_timer;

public: 
    
    Unity(int argc, char *argv[]);
    const std::vector<nui::UserStatus>& detectUsers();
    std::vector<std::string> recogWords(int timeout);
    bool isValid() const;
    void waitStatusUpdate();
    
};
