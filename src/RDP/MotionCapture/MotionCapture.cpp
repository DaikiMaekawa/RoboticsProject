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
	connectSignals();
	m_userRecog.setRGBCb(boost::bind(&MotionCapture::rgbImageCb, this, boost::lambda::_1));
	m_userRecog.setDetectUsersCb(boost::bind(&MotionCapture::detectUsersCb, this, boost::lambda::_1));
	//m_userRecog.setDepthCb(boost::bind(&MotionCapture::depthImageCb, this, boost::lambda::_1));
	m_timer.start(20);
}

/*
void MotionCapture::depthImageCb(const sensor_msgs::ImageConstPtr &image){
	std::cout << "MotionCreator->depthImageCb()" << std::endl;
	if(image->width > 0 && image->height > 0){
		QImage qimg(image->width, image->height, QImage::Format_RGB888);
		memcpy(qimg.scanLine(0), &image->data[0], image->width * image->height * 1);
		m_ui->labelImage->setPixmap(QPixmap::fromImage(qimg.rgbSwapped()));
	}
}
*/

void MotionCapture::detectUsersCb(const RDP::DetectUsersConstPtr &users){
	std::cout << "UserNum = " << users->data.size() << std::endl;
}

void MotionCapture::rgbImageCb(const sensor_msgs::ImageConstPtr& image){
	if(image->width > 0 && image->height > 0){
		QImage qimg(image->width, image->height, QImage::Format_RGB888);
		memcpy(qimg.scanLine(0), &image->data[0], image->width * image->height * 3);
		m_ui->labelImage->setPixmap(QPixmap::fromImage(qimg.rgbSwapped()));
	}
}

void MotionCapture::connectSignals(){
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(updateStatus()));
}

void MotionCapture::updateStatus(){
	ros::spinOnce();
}

MotionCapture::~MotionCapture()
{
    delete m_ui;
}
