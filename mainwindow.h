#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
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
    void showImg();
    void run();
    void single_run();
    QImage MatImageToQt(const cv::Mat &src);

private:
    Ui::MainWindow  *ui;
    entrance        *yolov5s_entrance;
    uart            *ser;
    QTimer          *timer;
    cv::Mat         one_img;
    int             file_num;
    int             id;
    std::vector<std::string> labels;
    std::vector<cv::String> file_names;
};

#endif // MAINWINDOW_H
