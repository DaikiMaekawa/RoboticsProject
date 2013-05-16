#pragma once
#include <QObject>
#include <RDP/UserStatus.h>
#include "Motion.h"

namespace Ui {
	class MotionCapture;
}

class PoseManeger : public QObject
{
	Q_OBJECT

public:
	PoseManeger(Ui::MotionCapture *ui);
	void detectUsersCb(const RDP::DetectUsersConstPtr &users);

private slots:
	void onChangeId(int id);
	void onPushSave();

private:
	int m_currentId;
	Ui::MotionCapture *m_ui;
	Motion m_motion;
	RDP::UserStatus m_currentUserStatus;
	void connectSignals();

};

