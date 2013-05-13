#include "MotionCapture.h"
#include "ui_MotionCapture.h"
#include <boost/lambda/lambda.hpp>
#include <boost/bind.hpp>
#include <iostream>

MotionCapture::MotionCapture(ros::NodeHandle &node, QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MotionCapture),
	m_userRecog(node)
{
    m_ui->setupUi(this);
	m_userRecog.setRGBCb(boost::bind(&MotionCapture::userRGBImageCb, this, boost::lambda::_1));
}

void MotionCapture::userRGBImageCb(const cv_bridge::CvImageConstPtr& image){
	std::cout << "MotionCreator->userRGBImageCb()" << std::endl;	
}

MotionCapture::~MotionCapture()
{
    delete m_ui;
}
