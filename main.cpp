#include "mainwindow.h"
#include <QApplication>

static void show_userge(std::string name){
    std::cout << "Usage: " << name << " <option(s)> SOURCES"
            << "Options:\n"
            << "\t-h,--help\t\tShow this help message\n"
            << "\t-j,--json\t\tinput json file config\n"
            << std::endl;
}

int main(int argc, char *argv[])
{
    if(argc<2){
        show_userge(argv[0]);
        return 0;
    }
    std::string json_path = "";
    for(int i=1; i<argc; i++){
        std::string arg = argv[i++];
        if(arg == "-h" || arg == "--help"){
            show_userge(argv[0]);
            return 0;
        }else if(arg == "-j" || arg == "--json"){
            if(i<argc){
                json_path = argv[i];
            }else{
                LOG(ERROR)<<"-j need input json path";
                return 0;
            }
        }else{
            show_userge(argv[0]);
            return 0;
        }
    }
    QApplication a(argc, argv);
    MainWindow w(json_path);
    w.show();

    return a.exec();
}
