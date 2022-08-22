#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // yolov5s = new entrance(0, 128, 128, 3);
    // ser = new uart(8, 32);
    // bool flag = ser->uartInit((char*)"/dev/ttyAMA0", 115200);
    // assert(!flag); 
    // LOG(INFO)<<"init ok";

    //single pic
    // yolov5s->run_cam();
    // yolov5s->set_input();
    // yolov5s->run();
    // std::vector<ground_truth> output = yolov5s->get_output();
    // yolov5s->print_output();  
    // run();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::run(){   
    while (1)    
    {   
        yolov5s->run_cam();
        bool uart_flag = ser->uartRecv();    
        if(uart_flag && ser->isCorrect()){  
            yolov5s->set_input();
            yolov5s->run();
            std::vector<ground_truth> output = yolov5s->get_output();
            std::vector<std::string> labels = yolov5s->get_label();
            ser->uartSend(output, labels);
            yolov5s->print_output();
        }           
    } 
}