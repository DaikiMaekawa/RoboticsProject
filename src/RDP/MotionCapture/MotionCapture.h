#ifndef MOTIONCAPTURE_H
#define MOTIONCAPTURE_H

#include <QMainWindow>
#include "UserRecognitionClient.h"
#include "PoseManeger.h"
#include <sensor_msgs/Image.h>
#include <ros/ros.h>
#include <QTimer>
#include <QGraphicsScene>

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

private:
    Ui::MotionCapture *m_ui;
	QGraphicsScene m_scene;
	PoseManeger m_poseManeger;
	nui::UserRecognitionClient m_userRecog;
	QTimer m_timer;
	void rgbImageCb(const sensor_msgs::ImageConstPtr &image);
	void connectSignals();
};

#endif // MOTIONCAPTURE_H

