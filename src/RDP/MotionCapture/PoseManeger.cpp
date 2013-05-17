#include "PoseManeger.h"
#include "ui_MotionCapture.h"
#include <boost/lambda/lambda.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <QFileDialog>

PoseManeger::PoseManeger(Ui::MotionCapture *ui) : 
	m_ui(ui)
{

}

void PoseManeger::setUserStatus(const RDP::UserStatus &currentStatus){
	m_currentUserStatus = currentStatus;
}

void PoseManeger::saveUserStatus(){
	m_motion.setUserStatus(m_ui->spinMotionId->value(), m_currentUserStatus);
}

void PoseManeger::saveMotion(const QString &file){
	m_motion.saveAs(file.toStdString());
}

void PoseManeger::loadMotion(const QString &file){
	m_motion.loadFrom(file.toStdString());
}

