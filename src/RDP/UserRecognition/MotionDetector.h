/*******************************************************************
 * Copyright (c) 2013 Daiki Maekawa
 *
 * @file MotionDetector.h
 * @brief Motion detection of user
 * @author Daiki Maekawa
 * @date 2013-04-18
 *******************************************************************/

#pragma once

#include <vector>
#include "Motion.h"
#include <string>

namespace nui{

class MotionDetector{
	std::vector<Motion> m_motions;
	const Motion& motion(unsigned int id);

public:
	void loadFromMotionFiles(const std::string &dirpath);
	void detectAll();
	void detect(unsigned int id);

};

} //namespace nui

