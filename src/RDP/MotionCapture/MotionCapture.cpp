#include "MotionCapture.h"
#include "ui_MotionCapture.h"

MotionCapture::MotionCapture(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MotionCapture)
{
    ui->setupUi(this);
}

MotionCapture::~MotionCapture()
{
    delete ui;
}
