#include "PoseManeger.h"
#include "ui_MotionCapture.h"
#include <boost/lambda/lambda.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <QFileDialog>
#include <boost/tuple/tuple.hpp>
#include <cassert>

PoseManeger::PoseManeger(Ui::MotionCapture *ui) : 
	m_ui(ui),
	m_joints(Motion::MAX_JOINT_NUM)
{
	initJoints();
	connectSignals();
}

void PoseManeger::setUserStatus(const RDP::UserStatus &currentStatus){
	assert(Motion::MAX_JOINT_NUM == currentStatus.joints.size());
	m_currentUserStatus = currentStatus;
}

/*
void PoseManeger::saveUserStatus(QGraphicsScene *scene){
	unsigned int poseId = m_ui->spinPoseId->value();
	m_scenes.insert(PoseScene::value_type(poseId, scene));
	//m_motion.setUserStatus(poseId, m_currentUserStatus);
	m_ui->graphicsStorage->setScene(scene);
}
*/
/*
void PoseManeger::saveMotion(const QString &file){
	m_motion.saveAs(file.toStdString());
}

void PoseManeger::loadMotion(const QString &file){
	m_motion.loadFrom(file.toStdString());
}
*/
void PoseManeger::onPushReadPose(){
	
	/*
	for(int i=0; i < Motion::MAX_JOINT_NUM; i++){
		m_spinboxes[i]->setValue(m_currentUserStatus.joints[i]);
	}
	*/

}

void PoseManeger::onPushSavePose(){
	std::cout << "onPushSavePose" << std::endl;
	m_motion.setPose(m_ui->spinPoseId->value(), m_joints);
}

void PoseManeger::onActionSave(){
	QString file = QFileDialog::getSaveFileName(this, "Save File", "/home", "Motion file (*.txt)");
	m_motion.saveAs(file.toStdString());
}

void PoseManeger::onActionLoad(){
	QString file = QFileDialog::getOpenFileName(this, "Open File", "/home", "Motion file (*.txt)");
	m_motion.loadFrom(file.toStdString());

	m_joints = m_motion.pose(m_ui->spinPoseId->value());
	assert(m_joints.size() == Motion::MAX_JOINT_NUM);
	showStorageJoints();
}

void PoseManeger::onSpinPoseidChanged(int id){
	
	m_joints = m_motion.pose(id);
	assert(m_joints.size() == Motion::MAX_JOINT_NUM);
	showStorageJoints();
	//TODO change scene
	//m_ui->graphicsStorage->setScene();
}

void PoseManeger::onSpinJointChanged(double value){
	std::cout << "onSpinJointChanged" << std::endl;
	assert(Motion::MAX_JOINT_NUM == m_spinboxes.size());
	
	for(int i=0; i < Motion::MAX_JOINT_NUM; i++){
		if((QObject*)m_spinboxes[i] == sender()){
			if(m_ui->radioX->isChecked()){
				m_joints[i].pos.x = m_spinboxes[i]->value();
				return;
			}else if(m_ui->radioY->isChecked()){
				m_joints[i].pos.y = m_spinboxes[i]->value();
				return;
			}else if(m_ui->radioZ->isChecked()){
				m_joints[i].pos.z = m_spinboxes[i]->value();
				return;
			}else{
				assert(false);
			}
		}
	}

	assert(false);
}

void PoseManeger::onCheckJointChanged(int state){
	assert(Motion::MAX_JOINT_NUM == m_checkboxes.size());

	for(int i=0; i < Motion::MAX_JOINT_NUM; i++){
		if((QObject*)m_checkboxes[i] == sender()){
			if(m_ui->radioX->isChecked()){
				std::cout << "m_joints[" << i << "].xIsKey = " << (state == Qt::Checked) << std::endl;				
				m_joints[i].xIsKey = (state == Qt::Checked);
				return;
			}else if(m_ui->radioY->isChecked()){
				std::cout << "m_joints[" << i << "].yIsKey = " << (state == Qt::Checked) << std::endl;				
				m_joints[i].yIsKey = (state == Qt::Checked);
				return;
			}else if(m_ui->radioZ->isChecked()){
				std::cout << "m_joints[" << i << "].zIsKey = " << (state == Qt::Checked) << std::endl;				
				m_joints[i].zIsKey = (state == Qt::Checked);
				return;
			}else{
				assert(false);
			}
		}
	}

	assert(false);
}

void PoseManeger::connectSignals(){
	connect(m_ui->spinPoseId, SIGNAL(valueChanged(int)), this, SLOT(onSpinPoseidChanged(int)));
	connect(m_ui->pushReadPose, SIGNAL(clicked()), this, SLOT(onPushReadPose()));
	connect(m_ui->pushSavePose, SIGNAL(clicked()), this, SLOT(onPushSavePose()));
	connect(m_ui->actionSave, SIGNAL(triggered()), this, SLOT(onActionSave()));
	connect(m_ui->actionLoad, SIGNAL(triggered()), this, SLOT(onActionLoad()));
	connect(m_ui->radioX, SIGNAL(clicked()), this, SLOT(showStorageJoints()));
	connect(m_ui->radioY, SIGNAL(clicked()), this, SLOT(showStorageJoints()));
	connect(m_ui->radioZ, SIGNAL(clicked()), this, SLOT(showStorageJoints()));
	for(int i=0; i < Motion::MAX_JOINT_NUM; i++){
		connect(m_spinboxes[i], SIGNAL(valueChanged(double)), this, SLOT(onSpinJointChanged(double)));
		connect(m_checkboxes[i], SIGNAL(stateChanged(int)), this, SLOT(onCheckJointChanged(int)));
	}
}

void PoseManeger::initJoints(){
	m_spinboxes << m_ui->spinHead;
	m_spinboxes << m_ui->spinNeck;
	m_spinboxes << m_ui->spinTorso;
	m_spinboxes << m_ui->spinElbowL;
	m_spinboxes << m_ui->spinElbowR;
	m_spinboxes << m_ui->spinHipL;
	m_spinboxes << m_ui->spinHipR;
	m_spinboxes << m_ui->spinKneeL;
	m_spinboxes << m_ui->spinKneeR;
	m_spinboxes << m_ui->spinHandL;
	m_spinboxes << m_ui->spinHandR;
	m_spinboxes << m_ui->spinShoulderL;
	m_spinboxes << m_ui->spinShoulderR;
	m_spinboxes << m_ui->spinFootL;
	m_spinboxes << m_ui->spinFootR;
	
	m_checkboxes << m_ui->checkHead;
	m_checkboxes << m_ui->checkNeck;
	m_checkboxes << m_ui->checkTorso;
	m_checkboxes << m_ui->checkElbowL;
	m_checkboxes << m_ui->checkElbowR;
	m_checkboxes << m_ui->checkHipL;
	m_checkboxes << m_ui->checkHipR;
	m_checkboxes << m_ui->checkKneeL;
	m_checkboxes << m_ui->checkKneeR;
	m_checkboxes << m_ui->checkHandL;
	m_checkboxes << m_ui->checkHandR;
	m_checkboxes << m_ui->checkShoulderL;
	m_checkboxes << m_ui->checkShoulderR;
	m_checkboxes << m_ui->checkFootL;
	m_checkboxes << m_ui->checkFootR;
}

void PoseManeger::showStorageJoints(){
	for(int i=0; i < Motion::MAX_JOINT_NUM; i++){
		if(m_ui->radioX->isChecked()){
			m_spinboxes[i]->setValue(m_joints[i].pos.x);
			m_checkboxes[i]->setCheckState(m_joints[i].xIsKey ? Qt::Checked : Qt::Unchecked);
		}else if(m_ui->radioY->isChecked()){
			m_spinboxes[i]->setValue(m_joints[i].pos.y);
			m_checkboxes[i]->setCheckState(m_joints[i].yIsKey ? Qt::Checked : Qt::Unchecked);
		}else if(m_ui->radioZ->isChecked()){
			m_spinboxes[i]->setValue(m_joints[i].pos.z);
			m_checkboxes[i]->setCheckState(m_joints[i].zIsKey ? Qt::Checked : Qt::Unchecked);
		}else{
			assert(false);
		}
	}
}


