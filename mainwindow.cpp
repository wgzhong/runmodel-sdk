#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QImage>
#include <QDebug>
#include "time/cal_time.h"
#include "macro_debug/debug.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    yolov5s_entrance = new entrance("/home/wgzhong/runmodel-sdk/test/yolov5s/yolov5s.json");
    std::vector<std::string> labels = yolov5s_entrance->get_label();
    #if DEBUG_VIDOE
    ser = new uart(8, 32);
    bool flag = ser->uartInit((char*)"/dev/ttyAMA0", 115200);
    assert(flag);
    LOG(INFO)<<"init ok";
    #elif DEBUG_PIC
    //single pic
    std::vector<cv::String> file_names;
    int file_num = yolov5s_entrance->get_filenames(file_names);
    if(file_num < 1){
        LOG(ERROR)<<"images array size is None";
    }
    
    for(int i=0; i<file_num; i++){
        cv::Mat src = yolov5s_entrance->run_cam(file_names[i]);
        LOG(INFO)<<file_num<<" "<<file_names[i];
        cv::Mat dst; 
        cv::cvtColor(src, dst, cv::COLOR_BGR2RGB);
        QImage disImage = QImage((const unsigned char*)(dst.data), dst.cols, dst.rows,  dst.cols*dst.channels(), QImage::Format_RGB888);
        ui->view_img->setPixmap(QPixmap::fromImage(disImage.scaled(ui->view_img->size(), Qt::KeepAspectRatio)));  // label 显示图像
        long start_time = getTimeUsec();
        yolov5s_entrance->set_input();
        yolov5s_entrance->run();
        long t1 = (getTimeUsec() - start_time) / 1000;
        std::vector<ground_truth> output = yolov5s_entrance->get_output();
        yolov5s_entrance->print_output();
        ui->time->setText(QString::number(t1)+" ms");
        ui->prob->setText(QString("%1").arg(output[0].class_prob));
        ui->label_id->setText(QString::number(output[0].label_idx));
        ui->classes->setText(QString::fromStdString(labels[output[0].label_idx]));
    } 
    #endif
    #if DEBUG_VIDOE
    run();
    #endif
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::run(){
    while (1)
    {
       yolov5s_entrance->run_cam();
       bool uart_flag = ser->uartRecv();
       if(uart_flag && ser->isCorrect()){
            long start_time = getTimeUsec();
            yolov5s_entrance->set_input();
            yolov5s_entrance->run();
            long t1 = (getTimeUsec() - start_time) / 1000;
            std::vector<ground_truth> output = yolov5s_entrance->get_output();
            std::vector<std::string> labels = yolov5s_entrance->get_label();
            ser->uartSend(output, labels);
            ui->time->setText(QString::number(t1)+" ms");
            ui->prob->setText(QString("%1").arg(output[0].class_prob));
            ui->label_id->setText(QString::number(output[0].label_idx));
            ui->classes->setText(QString::fromStdString(labels[output[0].label_idx]));
        //    yolov5s_entrance->print_output();
       }
    }
}
