#include <QApplication>
#include "MotionCapture.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MotionCapture w;
    w.show();
    
    return a.exec();
}

