#pragma once
#include <QWidget>
#include <RDP/UserStatus.h>
#include "Motion.h"

namespace Ui {
	class MotionCapture;
}

class PoseManeger : public QWidget
{
	Q_OBJECT

public:
	PoseManeger(Ui::MotionCapture *ui);
	void setUserStatus(const RDP::UserStatus &currentStatus);
	void saveUserStatus();
	void saveMotion(const QString &file);
	void loadMotion(const QString &file);

private:
	Ui::MotionCapture *m_ui;
	Motion m_motion;
	RDP::UserStatus m_currentUserStatus;

};

