#include "MotionCapture.h"
#include "ui_MotionCapture.h"
#include <RDP/UserStatus.h>
#include <QGraphicsEllipseItem>
#include <boost/lambda/lambda.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <QFileDialog>

MotionCapture::MotionCapture(ros::NodeHandle &node, QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MotionCapture),
	m_userRecog(node)
{
    m_ui->setupUi(this);
	m_poseManager = boost::shared_ptr<PoseManager>(new PoseManager(m_ui));
	connectSignals();
	//m_userRecog.setRGBCb(boost::bind(&MotionCapture::rgbImageCb, this, boost::lambda::_1));
    m_userRecog.setDepthCb(boost::bind(&MotionCapture::rgbImageCb, this, boost::lambda::_1));
    m_userRecog.setDetectUsersCb(boost::bind(&MotionCapture::detectUsersCb, this, boost::lambda::_1));
	m_timer.start(5);
}

void MotionCapture::rgbImageCb(const sensor_msgs::ImageConstPtr& image){
    std::cout << "rgbImageCb" << std::endl;
	if(image->width > 0 && image->height > 0){
		QImage qimg(image->width, image->height, QImage::Format_RGB888);
        memcpy(qimg.scanLine(0), &image->data[0], image->width * image->height * 3);
		
		m_scene.setSceneRect(m_ui->graphicsCurrent->viewport()->geometry());
		m_scene.setBackgroundBrush(QPixmap::fromImage(qimg.rgbSwapped()));
	}
}

void MotionCapture::detectUsersCb(const RDP::DetectUsersConstPtr &users){
	std::cout << "detectUsersCb" << std::endl;
	
	for(int i=0; i < users->data.size(); i++){
		if(m_ui->spinUserId->value() == users->data[i].id){
			std::cout << "SelectUserStatusId = " << users->data[i].id << std::endl;
			m_currentUserStatus = users->data[i];
			paintUserJoints(users->data[i]);

			return;
		}
	}
}

void MotionCapture::connectSignals(){
	bool ret;
    ret = connect(&m_timer, SIGNAL(timeout()), this, SLOT(updateStatus()));
	assert(ret);
    ret = connect(m_ui->pushReadPose, SIGNAL(clicked()), this, SLOT(onPushReadPose()));
    assert(ret);
}

void MotionCapture::updateStatus(){
	ros::spinOnce();
	m_ui->graphicsCurrent->setScene(&m_scene);
}

void MotionCapture::onPushReadPose(){
	m_poseManager->setStoragePose(m_currentUserStatus.pose);
}

void MotionCapture::paintUserJoints(const RDP::UserStatus &user){
	for(int i=0; i < m_gItems.size(); i++){
		m_scene.removeItem(m_gItems[i]);
		delete m_gItems[i];
	}
	m_gItems.clear();
	
	for(int i=0; i < user.pose.joints.size(); i++){
		QColor color(255, 0, 0);
		QPen pen(color);
		QBrush brush(color);

		std::cout << "joints[" << i << "][x,y] = " << user.pose.joints[i].pos.x << "," << user.pose.joints[i].pos.y << std::endl;

		m_gItems << static_cast<QGraphicsItem*>(m_scene.addEllipse(user.pose.joints[i].pos.x, user.pose.joints[i].pos.y, 16, 16, pen, brush));
	}
}

MotionCapture::~MotionCapture()
{
    delete m_ui;
}

