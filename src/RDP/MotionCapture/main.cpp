#include <QApplication>
#include "MotionCapture.h"
#include <ros/ros.h>

int main(int argc, char *argv[])
{
	ros::init(argc, argv, "MotionCapture");
	ros::NodeHandle node;
    QApplication a(argc, argv);
	MotionCapture w(node);
    w.show();
    
    return a.exec();
}

