#ifndef MOTIONCAPTURE_H
#define MOTIONCAPTURE_H

#include <QMainWindow>

namespace Ui {
class MotionCapture;
}

class MotionCapture : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MotionCapture(QWidget *parent = 0);
    ~MotionCapture();
    
private:
    Ui::MotionCapture *ui;
};

#endif // MOTIONCAPTURE_H

