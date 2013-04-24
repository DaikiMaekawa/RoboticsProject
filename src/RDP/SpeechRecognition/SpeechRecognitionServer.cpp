#include "SpeechRecognitionServer.h"
#include "SpeechRecognitionClient.h"
#include <std_msgs/String.h>
#include <boost/lambda/lambda.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <unicode/unistr.h>
extern "C"{ 
    #include <julius/juliuslib.h> 
}
using namespace std;

namespace nui{

    SpeechRecognitionServer::SpeechRecognitionServer(int argc, char *argv[], ros::NodeHandle &node) : 
        m_isValid(initJulius(argc, argv)),
	m_node(node),
        m_isOpenStream(false),
        m_isPausedStream(false),
        m_resultPub(m_node.advertise<std_msgs::String>(RESULT_SPEECH_RECOGNITION, 100)),
	m_startSub(m_node.subscribe(START_SPEECH_RECOGNITION, 100, &SpeechRecognitionServer::startRecognition, this)),
	m_pauseSub(m_node.subscribe(PAUSE_SPEECH_RECOGNITION, 100, &SpeechRecognitionServer::pauseRecognition, this)),
	m_finishSub(m_node.subscribe(FINISH_SPEECH_RECOGNITION, 100, &SpeechRecognitionServer::finishRecognition, this))
    {
        sleep(1);
    }

    bool SpeechRecognitionServer::initJulius(int argc, char *argv[]){
        const int insArgc = 3;
	char *insArgv[3];
	insArgv[0] = argv[0];
	insArgv[1] = "-C";
	string insStr = std::string(SHARE_DIR) + "/julius.jconf";
	insArgv[2] = &insStr[0];
	cout << "julius load file = " << insArgv[2] << endl;

        Jconf *jconf = j_config_load_args_new(insArgc, insArgv);

        if(jconf == NULL) return false;
        m_recog = boost::shared_ptr<Recog>(j_create_instance_from_jconf(jconf),boost::bind(j_recog_free, boost::lambda::_1));
        if(m_recog == NULL) return false;

        callback_add(m_recog.get(), CALLBACK_RESULT,&SpeechRecognitionServer::callWhenResult, this);
        callback_add(m_recog.get(), CALLBACK_EVENT_PAUSE, &SpeechRecognitionServer::callInPause, this);
        callback_add(m_recog.get(), CALLBACK_EVENT_RESUME, &SpeechRecognitionServer::callWhenResume, this);

        return j_adin_init(m_recog.get()) != FALSE;
    }

    void SpeechRecognitionServer::finishRecognition(const std_msgs::EmptyConstPtr &msg){
        if(m_isOpenStream){
            m_isOpenStream = false;
            j_close_stream(m_recog.get()); 
	}
    }

    void SpeechRecognitionServer::pauseRecognition(const std_msgs::EmptyConstPtr &msg){
        if(m_isOpenStream) j_request_pause(m_recog.get());
    }

    void SpeechRecognitionServer::callInPause(Recog *recog, void *receiver){
        SpeechRecognitionServer* srObj = (SpeechRecognitionServer*)receiver;
        srObj->m_isPausedStream = true;
        while(true){
            atlas::Timer::sleep(1);
            cout << "callInPause" << endl;
        }
    }

    void SpeechRecognitionServer::callWhenResume(Recog *recog, void *receiver){
        SpeechRecognitionServer* srObj = (SpeechRecognitionServer*)receiver;
        srObj->m_isPausedStream = false;
        cout << "callWhenResume" << endl;
    }

    void SpeechRecognitionServer::callWhenResult(Recog *recog, void *receiver){
        std::cout << "callWhenResult" << std::endl;
        SpeechRecognitionServer* srObj = (SpeechRecognitionServer*)receiver;
        
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
                    /*
		    icu::UnicodeString src(winfo->woutput[seq[i]], "euc-jp");
                    const int32_t len = src.extract(0, src.length(), NULL, "utf8");
                    char* woutput = new char[len + 1];
                    src.extract(0, src.length(), woutput, "utf8");
                    */
		    string woutput = srObj->stringConvert(string(winfo->woutput[seq[i]]), "euc-jp", "utf8");
		    cout << "woutput[" << i << "] = " << woutput << endl;
                    //ret += string(woutput);
                    ret += woutput;
		    //delete []woutput;
                }

                cout << "Result String" << n+1 << " = " << ret << endl;
                std_msgs::String msg;
		msg.data = ret;
		srObj->m_resultPub.publish(msg);
            }
        }
    }

    string SpeechRecognitionServer::stringConvert(const string &src, const char *srcEnc, const char *retEnc){
        icu::UnicodeString unicode(src.c_str(), srcEnc);
	const int32_t len = unicode.extract(0, unicode.length(), NULL, retEnc);
	char *ins = new char[len + 1];
	unicode.extract(0, unicode.length(), ins, retEnc);
        string ret = ins;
	delete []ins;

	return ret;
    }

    void SpeechRecognitionServer::startRecognition(const std_msgs::EmptyConstPtr &msg){
        ROS_INFO("StartSpeechRecognition");     
        if(m_isPausedStream){
            j_request_resume(m_recog.get());
            return;
        }

        switch(j_open_stream(m_recog.get(), NULL)) {
        case 0:	
            m_isOpenStream = true;
            break;
        case -1:      		
            fprintf(stderr, "error in input stream\n");
            return;
        case -2:			
            fprintf(stderr, "failed to begin input stream\n");
            return;
        }

        //m_timer.singleShot(timeout, boost::bind(&SpeechRecognition::finishRecognition, this));
        j_recognize_stream(m_recog.get());

    }

    void SpeechRecognitionServer::runServer(){
        ros::Rate loopRate(5);
	while(ros::ok()){
            ros::spinOnce();
	    loopRate.sleep();
	}
    }

} //namespace nui

