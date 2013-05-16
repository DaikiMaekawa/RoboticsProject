#include "MotionCapture.h"
#include "ui_MotionCapture.h"
#include <RDP/UserStatus.h>
#include <QGraphicsEllipseItem>
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
	m_userRecog.setDetectUsersCb(boost::bind(&MotionCapture::detectUsersCb, this, boost::lambda::_1));
	m_timer.start(10);
}

void MotionCapture::rgbImageCb(const sensor_msgs::ImageConstPtr& image){
	if(image->width > 0 && image->height > 0){
		QImage qimg(image->width, image->height, QImage::Format_RGB888);
		memcpy(qimg.scanLine(0), &image->data[0], image->width * image->height * 3);
		
		m_scene.setSceneRect(m_ui->graphicsView->viewport()->geometry());
		m_scene.setBackgroundBrush(QPixmap::fromImage(qimg.rgbSwapped()));
	}
}

void MotionCapture::detectUsersCb(const RDP::DetectUsersConstPtr &users){
	std::cout << "detectUsersCb" << std::endl;
	
	for(int i=0; i < users->data.size(); i++){
		if(m_ui->spinUserId->value() == users->data[i].id){
			std::cout << "SelectUserStatusId = " << users->data[i].id << std::endl;
			m_poseManeger.setUserStatus(users->data[i]);
			paintUserJoints(users->data[i]);

			return;
		}
	}
}

void MotionCapture::connectSignals(){
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(updateStatus()));
}

void MotionCapture::updateStatus(){
	ros::spinOnce();
	m_ui->graphicsView->setScene(&m_scene);
}

void MotionCapture::paintUserJoints(const RDP::UserStatus &user){
	for(int i=0; i < m_gItems.size(); i++){
		m_scene.removeItem(m_gItems[i]);
		delete m_gItems[i];
	}
	m_gItems.clear();
	
	for(int i=0; i < user.joints.size(); i++){
		QColor color(255, 255, 0);
		QPen pen(color);
		QBrush brush(color);

		std::cout << "joints[" << i << "][x,y] = " << user.joints[i].pos.x << "," << user.joints[i].pos.y << std::endl;

		m_gItems << static_cast<QGraphicsItem*>(m_scene.addEllipse(user.joints[i].pos.x, user.joints[i].pos.y, 8, 8, pen, brush));
	}
}

MotionCapture::~MotionCapture()
{
    delete m_ui;
}

