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
    explicit MainWindow(std::string json_path, QWidget *parent = 0);
    ~MainWindow();
    void showImg();
    void run_core();
    void run();
    void save_pic();
    QImage MatImageToQt(const cv::Mat &src);

private:
    Ui::MainWindow  *ui;
    entrance        *m_yolov5s_entrance;
    uart            *m_ser;
    QTimer          *m_timer;
    cv::Mat         m_one_img;
    int             m_file_num;
    std::vector<std::string> m_labels;
    std::vector<cv::String> m_file_names;
    config_def_t m_json_config;
};

#endif // MAINWINDOW_H
