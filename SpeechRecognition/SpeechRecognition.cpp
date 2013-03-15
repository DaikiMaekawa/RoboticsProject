#include "SpeechRecognition.h"
#include <boost/lambda/lambda.hpp>
#include <boost/bind.hpp>
#include <iostream>

using namespace std;

namespace nui{

    SpeechRecognition::SpeechRecognition(int argc, char *argv[]) : 
        m_isValid(initJulius(argc,argv)),
        m_isOpenStream(false),
        m_isPausedStream(false)
    {
        
    }

    bool SpeechRecognition::initJulius(int argc, char *argv[]){
        Jconf *jconf = j_config_load_args_new(argc,argv);

        if(jconf == NULL) return false;
        m_recog = boost::shared_ptr<Recog>(j_create_instance_from_jconf(jconf),boost::bind(j_recog_free, boost::lambda::_1));
        if(m_recog == NULL) return false;

        callback_add(m_recog.get(), CALLBACK_RESULT,&SpeechRecognition::callWhenResult, this);
        callback_add(m_recog.get(), CALLBACK_EVENT_PAUSE, &SpeechRecognition::callInPause, this);
        callback_add(m_recog.get(), CALLBACK_EVENT_RESUME, &SpeechRecognition::callWhenResume, this);

        return j_adin_init(m_recog.get()) != FALSE;
    }

    void SpeechRecognition::finishRecognition(){
        if(m_isOpenStream){
            m_isOpenStream = false;
            j_close_stream(m_recog.get());
        }
    }

    void SpeechRecognition::pauseRecognition(){
        if(m_isOpenStream) j_request_pause(m_recog.get());
    }

    void SpeechRecognition::callInPause(Recog *recog, void *receiver){
        SpeechRecognition* srObj = (SpeechRecognition*)receiver;
        srObj->m_isPausedStream = true;
        while(true){
            atlas::Timer::sleep(1);
            cout << "callInPause" << endl;
        }
    }

    void SpeechRecognition::callWhenResume(Recog *recog, void *receiver){
        SpeechRecognition* srObj = (SpeechRecognition*)receiver;
        srObj->m_isPausedStream = false;
        cout << "callWhenResume" << endl;
    }

    void SpeechRecognition::callWhenResult(Recog *recog, void *receiver){
        std::cout << "callWhenResult" << std::endl;
        SpeechRecognition* srObj = (SpeechRecognition*)receiver;
        
        for(RecogProcess *process = recog->process_list; process ; process = process->next){
            if(! process->live) continue;
            
            if(process->result.status < 0){
                switch(process->result.status){
                case J_RESULT_STATUS_REJECT_POWER:
                    cout << "<input rejected by power>" << endl;
                    break;
                case J_RESULT_STATUS_TERMINATE:
                    cout << "<input teminated by request>" << endl;
                    break;
                case J_RESULT_STATUS_ONLY_SILENCE:
                    std::cout << "<input rejected by decoder (silence input result)>" << std::endl;
                    break;
                case J_RESULT_STATUS_REJECT_GMM:
                    std::cout << "<input rejected by GMM>" << std::endl;
                    break;
                case J_RESULT_STATUS_REJECT_SHORT:
                    std::cout << "<input rejected by short input>" << std::endl;
                    break;
                case J_RESULT_STATUS_FAIL:
                    std::cout << "<search failed>" << std::endl;
                    break;
                }
                continue;
            }

            WORD_INFO *winfo = process->lm->winfo;
            
            for(int n = 0; n < process->result.sentnum; n++){
                Sentence *sentence = &(process->result.sent[n]);
                WORD_ID *seq = sentence->word;
                const int seqnum = sentence->word_num;
                
                string ret;
                for(int i=0; i < seqnum; i++){
                    cout << "woutput[" << i << "] = " << winfo->woutput[seq[i]] << endl;
                    ret += string(winfo->woutput[seq[i]]);
                }

                cout << "Result String" << n+1 << " = " << ret << endl;
                srObj->m_resultsString.push_back(ret);
            }
        }
    }

    bool SpeechRecognition::startRecognition(unsigned long timeout){
        
        if(m_isPausedStream){
            j_request_resume(m_recog.get());
            return true;
        }

        switch(j_open_stream(m_recog.get(), NULL)) {
        case 0:	
            m_isOpenStream = true;
            break;
        case -1:      		
            fprintf(stderr, "error in input stream\n");
            return false;
        case -2:			
            fprintf(stderr, "failed to begin input stream\n");
            return false;
        }

        m_timer.singleShot(timeout, boost::bind(&SpeechRecognition::finishRecognition, this));
        return j_recognize_stream(m_recog.get()) != -1;

    }

} //namespace nui

