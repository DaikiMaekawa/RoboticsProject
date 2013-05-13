#ifndef MOTIONCAPTURE_H
#define MOTIONCAPTURE_H

#include <QMainWindow>
#include "UserRecognitionClient.h"
#include <cv_bridge/cv_bridge.h>
#include <ros/ros.h>

namespace Ui {
class MotionCapture;
}

class MotionCapture : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MotionCapture(ros::NodeHandle &node, QWidget *parent = 0);
    ~MotionCapture();
    
private:
    Ui::MotionCapture *m_ui;
	nui::UserRecognitionClient m_userRecog;
	void userRGBImageCb(const cv_bridge::CvImageConstPtr& image);
};

#endif // MOTIONCAPTURE_H

