#ifndef MOTIONCAPTURE_H
#define MOTIONCAPTURE_H

#include <QMainWindow>
#include "UserRecognitionClient.h"
#include <RDP/DetectUsers.h>
#include <sensor_msgs/Image.h>
#include <ros/ros.h>
#include <QTimer>

namespace Ui {
class MotionCapture;
}

class MotionCapture : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MotionCapture(ros::NodeHandle &node, QWidget *parent = 0);
    ~MotionCapture();

private slots:
	void updateStatus();

private:
    Ui::MotionCapture *m_ui;
	nui::UserRecognitionClient m_userRecog;
	QTimer m_timer;
	void rgbImageCb(const sensor_msgs::ImageConstPtr &image);
	void detectUsersCb(const RDP::DetectUsersConstPtr &users);
	//void depthImageCb(const sensor_msgs::ImageConstPtr &image);
	void connectSignals();
};

#endif // MOTIONCAPTURE_H

