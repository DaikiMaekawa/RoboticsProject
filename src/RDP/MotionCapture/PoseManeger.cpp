#include "PoseManeger.h"
#include "ui_MotionCapture.h"
#include <boost/lambda/lambda.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <QFileDialog>

PoseManeger::PoseManeger(Ui::MotionCapture *ui) : 
	m_ui(ui),
	m_currentId(0)
{
}

void PoseManeger::onChangeId(int id){
	m_currentId = id;	
}

void PoseManeger::onPushSave(){
	//QString file = QFileDialog::getSaveFileName(m_ui->centralWidget, "Save File", "/", "Motion file (*.txt)");
	m_motion.setUserStatus(m_currentId ,m_currentUserStatus);
	//m_motion.saveAs(file.toStdString());	
}

void PoseManeger::detectUsersCb(const RDP::DetectUsersConstPtr &users){
	std::cout << "detectUsersCb" << std::endl;

	for(int i=0; i < users->data.size(); i++){
		if(m_ui->spinUserId->value() == users->data[i].id){
			std::cout << "SelectUserStatusId = " << users->data[i].id << std::endl;
			m_currentUserStatus = users->data[i];
			
			return;
		}
	}
}

void PoseManeger::connectSignals(){
	connect(m_ui->spinMotionId, SIGNAL(valueChanged(int)), this, SLOT(onChangeId(int)));
	connect(m_ui->pushSavePose, SIGNAL(clicked()), this, SLOT(onPushSave()));
}

