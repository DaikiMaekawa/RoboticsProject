#include "MotionDetector.h"
#include <boost/filesystem.hpp>
#include <boost/tokenizer.hpp>
//#include <sstream>
//#include <fstream>
#include <iostream>

using namespace std;
using namespace boost::filesystem;

namespace nui{

const Motion& MotionDetector::motion(unsigned int id){
	for(int i=0; i < m_motions.size(); i++){
		if(id == m_motions[i].id()){
			return m_motions[i];
		}
	}

	assert(false);
}

void MotionDetector::loadFromMotionFiles(const string &dirpath){
	vector<path> fileVec;
	path dir(dirpath);
	directory_iterator end;

	if(exists(dir)){
		copy(directory_iterator(dir), directory_iterator(), back_inserter(fileVec));

		for(int i=0; i < (int)fileVec.size(); i++){
			path &p = fileVec[i];
			if(is_directory(p)){

			}else{
				string fleaf = p.filename().string();
				string ext = p.extension().string();

				if(ext == ".txt"){
					Motion motion;
					motion.loadFrom(p.filename().string());
					m_motions.push_back(motion);
				}
			}
		}
	}
}

void MotionDetector::detectAll(){
	
}

void MotionDetector::detect(unsigned int id){

}

} //namespace nui
