#ifndef MOTIONCAPTURE_H
#define MOTIONCAPTURE_H

#include <QMainWindow>
#include "UserRecognitionClient.h"
#include "PoseManeger.h"
#include <sensor_msgs/Image.h>
#include <ros/ros.h>
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
	//void onPushSavePose();
	//void onSaveFile();
	//void onLoadFile();

private:
    Ui::MotionCapture *m_ui;
	QVector<QGraphicsItem*>  m_gItems;
	QGraphicsScene m_scene;
	boost::shared_ptr<PoseManeger> m_poseManeger;
	nui::UserRecognitionClient m_userRecog;
	QTimer m_timer;
	void rgbImageCb(const sensor_msgs::ImageConstPtr &image);
	void detectUsersCb(const RDP::DetectUsersConstPtr &users);
	void connectSignals();
	void paintUserJoints(const RDP::UserStatus &user);
};

#endif // MOTIONCAPTURE_H

