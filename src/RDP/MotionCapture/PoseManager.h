#pragma once
#include <RDP/UserStatus.h>
#include <RDP/UserPose.h>
#include "Motion.h"
#include <map>
#include <vector>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QString>
#include <QVector>
#include <QGraphicsScene>
#include <QWidget>

//typedef std::map<unsigned int, QGraphicsScene*> PoseScene;

namespace Ui {
    class MotionCapture;
}

class PoseManager : public QWidget
{
    Q_OBJECT

public:
    PoseManager(Ui::MotionCapture *ui);
    void setStoragePose(const RDP::UserPose &pose);
    //void saveUserStatus(QGraphicsScene *scene);

private slots:
    void onPushSavePose();
    void onPushAddPose();
    void onActionSave();
    void onActionLoad();
    void onSpinPoseidChanged(int id);
    void onSpinJointChanged(double value);
    void onSpinTimeChanged(int value);
    void onCheckJointChanged(int state);
    void showStoragePose();

private:
    void showMessage(const QString &title, const QString &msg);
    void connectSignals();
    void initJoints();
    Ui::MotionCapture *m_ui;
    //PoseScene m_scenes;
    nui::Motion m_motion;
    QVector<QDoubleSpinBox*> m_spinboxes;
    QVector<QCheckBox*>      m_checkboxes;
    RDP::UserPose m_pose; 
    unsigned int m_poseId;

};

