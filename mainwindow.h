#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "test/yolov5s/test_yolov5s.h"
#include "protocol/uart.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void run();

private:
    Ui::MainWindow *ui;
    entrance *yolov5s_entrance;
    uart *ser;
};

#endif // MAINWINDOW_H
