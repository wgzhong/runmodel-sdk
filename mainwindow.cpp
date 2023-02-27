#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QImage>
#include <QDebug>
#include "time/cal_time.h"

MainWindow::MainWindow(std::string json_path, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{   
    if(!isFileExists_ifstream(json_path)){
        throw std::runtime_error("could not open json file " + json_path);
    }
    ui->setupUi(this);
    ui->view_img->setStyleSheet("QLabel {background-color: transparent;}");
    m_timer = new QTimer(this);
    get_json_content(json_path, m_json_config);

    connect(m_timer,&QTimer::timeout,this,&MainWindow::showImg);
    m_timer->start(1);
    m_yolov5s_entrance = new entrance(m_json_config);
    m_labels = m_yolov5s_entrance->get_label();
    if(m_json_config.m_pic_or_video == "pic"){
        m_file_num = m_yolov5s_entrance->get_filenames(m_file_names);
        if(m_file_num < 1){
            LOG(ERROR)<<"images array size is None";
        }
    } else if(m_json_config.m_pic_or_video == "video"){
        m_ser = new uart(8, 32);
        bool flag = m_ser->uartInit((char*)"/dev/ttyAMA0", 115200);
        assert(flag);
        LOG(INFO)<<"init ok";
        run();
    } else{
        LOG(ERROR)<<"mainwindow not support other format!\n";
    }
}

MainWindow::~MainWindow()
{
    m_timer->stop();
    delete ui;
}

void MainWindow::showImg(){
    run();
    if(!m_one_img.empty()){
        cv::Mat dst; 
        cv::Mat src = std::move(m_one_img);
        QImage disImage = MatImageToQt(src);
        ui->view_img->setPixmap(QPixmap::fromImage(disImage.scaled(ui->view_img->size(), Qt::KeepAspectRatio)));  // label 显示图像
        ui->view_img->update();
    } 
}

void MainWindow::run(){
    if(m_json_config.m_pic_or_video == "pic"){
        m_file_num = m_file_num-1;
        m_one_img = m_yolov5s_entrance->run_cam(m_file_names[m_file_num]);
        run_core();
        if(m_file_num == 0){
            LOG(INFO)<<"single test done!\n";
            m_timer->stop();
        }
    } else if(m_json_config.m_pic_or_video == "video"){
        m_one_img = m_yolov5s_entrance->run_cam();
        bool uart_flag = m_ser->uartRecv();
        if(uart_flag && m_ser->isCorrect()){
            run_core();
        }
    } else{
        LOG(ERROR)<<"mainwindow not support other format!\n";
    }
}
void MainWindow::run_core(){
    long start_time = getTimeUsec();
    if(m_json_config.m_save_pic == 1){
        char name[128];
        get_local_time(name);
        strcat(name, ".jpg");
        cv::imwrite(name, m_one_img);
    }
    m_yolov5s_entrance->set_input();
    m_yolov5s_entrance->run();
    long t1 = (getTimeUsec() - start_time) / 1000;
    std::vector<ground_truth> output = m_yolov5s_entrance->get_output();
    if(output.size() > 0){
        if(m_json_config.m_pic_or_video == "video"){
            m_ser->uartSend(output, m_labels);
        }
        ui->time->setText(QString::number(t1)+" ms");
        ui->prob->setText(QString("%1").arg(output[0].class_prob));
        ui->label_id->setText(QString::number(output[0].label_idx));
        ui->classes->setText(QString::fromStdString(m_labels[output[0].label_idx]));
    } else{
        ui->time->setText("not detection image!!!\n");
    }
}

//Mat to QImage
QImage MainWindow::MatImageToQt(const cv::Mat &src)
{
    //CV_8UC1 8位无符号的单通道---灰度图片
    if(src.type() == CV_8UC1)
    {
        //使用给定的大小和格式构造图像
        //QImage(int width, int height, Format format)
        QImage qImage(src.cols,src.rows,QImage::Format_Indexed8);
        //扩展颜色表的颜色数目
        qImage.setColorCount(256);

        //在给定的索引设置颜色
        for(int i = 0; i < 256; i ++)
        {
            //得到一个黑白图
            qImage.setColor(i,qRgb(i,i,i));
        }
        //复制输入图像,data数据段的首地址
        uchar *pSrc = src.data;
        //
        for(int row = 0; row < src.rows; row ++)
        {
            //遍历像素指针
            uchar *pDest = qImage.scanLine(row);
            //从源src所指的内存地址的起始位置开始拷贝n个
            //字节到目标dest所指的内存地址的起始位置中
            memcmp(pDest,pSrc,src.cols);
            //图像层像素地址
            pSrc += src.step;
        }
        return qImage;
    }
    //为3通道的彩色图片
    else if(src.type() == CV_8UC3)
    {
        //得到图像的的首地址
        const uchar *pSrc = (const uchar*)src.data;
        //以src构造图片
        QImage qImage(pSrc,src.cols,src.rows,src.step,QImage::Format_RGB888);
        //在不改变实际图像数据的条件下，交换红蓝通道
        return qImage.rgbSwapped();
    }
    //四通道图片，带Alpha通道的RGB彩色图像
    else if(src.type() == CV_8UC4)
    {
        const uchar *pSrc = (const uchar*)src.data;
        QImage qImage(pSrc, src.cols, src.rows, src.step, QImage::Format_ARGB32);
        //返回图像的子区域作为一个新图像
        return qImage.copy();
    }
    else
    {
        return QImage();
    }
}
