/*******************************************************************
 * Copyright (c) 2013 Daiki Maekawa
 *
 * @file MotionCapture.h
 * @brief motion capture ui
 * @author Daiki Maekawa
 * @date 2013-03-22
 *******************************************************************/

#pragma once
#ifndef MOTIONCAPTURE_H
#define MOTIONCAPTURE_H

#include <QMainWindow>
#include "UserRecognitionClient.h"
#include "PoseManager.h"
#include <sensor_msgs/Image.h>
#include <ros/ros.h>
#include <RDP/UserStatus.h>
#include <QTimer>
#include <QGraphicsScene>
#include <QVector>
#include <boost/shared_ptr.hpp>

namespace Ui {
	class MotionCapture;
}

class MotionCapture : public QMainWindow
{
    Q_OBJECT
    
public:
    MotionCapture(ros::NodeHandle &node, QWidget *parent = 0);
    ~MotionCapture();

private slots:
	void updateStatus();
	void onPushReadPose();

private:
    Ui::MotionCapture *m_ui;
	QVector<QGraphicsItem*>  m_gItems;
	QGraphicsScene m_scene;
	boost::shared_ptr<PoseManager> m_poseManager;
	nui::UserRecognitionClient m_userRecog;
	RDP::UserStatus m_currentUserStatus;
	QTimer m_timer;
	void rgbImageCb(const sensor_msgs::ImageConstPtr &image);
	void detectUsersCb(const RDP::DetectUsersConstPtr &users);
	void connectSignals();
	void paintUserJoints(const RDP::UserStatus &user);
};

#endif // MOTIONCAPTURE_H

