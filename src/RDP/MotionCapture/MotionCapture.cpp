#include "MotionCapture.h"
#include "ui_MotionCapture.h"
#include <boost/lambda/lambda.hpp>
#include <boost/bind.hpp>
#include <iostream>

MotionCapture::MotionCapture(ros::NodeHandle &node, QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MotionCapture),
	m_userRecog(node),
	m_poseManeger(m_ui)
{
    m_ui->setupUi(this);
	connectSignals();
	m_userRecog.setRGBCb(boost::bind(&MotionCapture::rgbImageCb, this, boost::lambda::_1));
	m_userRecog.setDetectUsersCb(boost::bind(&PoseManeger::detectUsersCb, &m_poseManeger, boost::lambda::_1));
	m_timer.start(10);
}

void MotionCapture::rgbImageCb(const sensor_msgs::ImageConstPtr& image){
	if(image->width > 0 && image->height > 0){
		QImage qimg(image->width, image->height, QImage::Format_RGB888);
		memcpy(qimg.scanLine(0), &image->data[0], image->width * image->height * 3);
		m_scene.setSceneRect(m_ui->graphicsView->viewport()->geometry());
		m_scene.setBackgroundBrush(QPixmap::fromImage(qimg.rgbSwapped()));
		m_ui->graphicsView->setScene(&m_scene);
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
