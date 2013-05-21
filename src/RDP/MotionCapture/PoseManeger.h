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

typedef std::map<unsigned int, QGraphicsScene*> PoseScene;
/*
enum Pos{
	X,
	Y,
	Z
};
*/

namespace Ui {
	class MotionCapture;
}

class PoseManeger : public QWidget
{
	Q_OBJECT

public:
	PoseManeger(Ui::MotionCapture *ui);
	void setUserStatus(const RDP::UserStatus &currentStatus);
	//void saveUserStatus(QGraphicsScene *scene);
	//void saveMotion(const QString &file);
	//void loadMotion(const QString &file);

private slots:
	void onPushReadPose();
	void onPushSavePose();
	void onActionSave();
	void onActionLoad();
	void onSpinPoseidChanged(int id);
	void onSpinJointChanged(double value);
	void onCheckJointChanged(int state);
	void showStorageJoints();

private:
	Ui::MotionCapture *m_ui;
	PoseScene m_scenes;
	Motion m_motion;
	QVector<QDoubleSpinBox*> m_spinboxes;
	QVector<QCheckBox*>      m_checkboxes;
	std::vector<RDP::UserJoint> m_joints;

	RDP::UserStatus m_currentUserStatus;
	//void setPoseScene(unsigned int poseId, const QGraphicsScene &scene);
	void connectSignals();
	void initJoints();

};

