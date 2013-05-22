#pragma once
#include <QWidget>
#include <RDP/UserStatus.h>
#include "Motion.h"
#include <QGraphicsScene>
#include <map>
#include <QVector>
#include <vector>
#include <QDoubleSpinBox>
#include <QCheckBox>

//typedef std::map<unsigned int, QGraphicsScene*> PoseScene;

namespace Ui {
	class MotionCapture;
}

class PoseManager : public QWidget
{
	Q_OBJECT

public:
	PoseManager(Ui::MotionCapture *ui);
	//void setStorageJoints(const std::vector<RDP::UserJoint> &joints);
	void setStoragePose(const RDP::UserPose &pose);
	//void saveUserStatus(QGraphicsScene *scene);

private slots:
	void onPushSavePose();
	void onActionSave();
	void onActionLoad();
	void onSpinPoseidChanged(int id);
	void onSpinJointChanged(double value);
	void onSpinTimeChanged(int value);
	void onCheckJointChanged(int state);
	void showStoragePose();

private:
	Ui::MotionCapture *m_ui;
	//PoseScene m_scenes;
	nui::Motion m_motion;
	QVector<QDoubleSpinBox*> m_spinboxes;
	QVector<QCheckBox*>      m_checkboxes;
	//std::vector<RDP::UserJoint> m_joints;
	//std::vector<RDP::UserPose> m_poses;
	RDP::UserPose m_pose; 

	//void setPoseScene(unsigned int poseId, const QGraphicsScene &scene);
	void connectSignals();
	void initJoints();

};

