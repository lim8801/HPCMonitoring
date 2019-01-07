#include "mainwindow.h"
#include "qcustomplot.h"
#include "ui_mainwindow.h"

int MainWindow::visualizer_Launch() {


    QString _launchArg1;
    QString _launchArg2;
    QString _launchArg3;
    QString _launchArg4;

    QStringList _arguments;

    _launchArg1 = "/usr/bin/java";
    _launchArg2 = "-cp";
    _launchArg3 = "/home/main/hylim/hpc-visualizer_yoon/bin:/home/main/hylim/hpc-visualizer_yoon/lib/*";
    _launchArg4 = "escal.hpc.FrontendMain";

    _arguments << _launchArg2;
    _arguments << _launchArg3;
    _arguments << _launchArg4;
    qDebug() << _arguments;

    if(QFile::exists(_launchArg1)){

        if(LaunchProcess == NULL){
            LaunchProcess = new QProcess(this);
        }

        LaunchProcess->start(_launchArg1,_arguments);
    }
    else{
        QMessageBox::information(this, "Error", "Visualizer is not found", "OK");
        return -1;
    }



/*
    Process = new QProcess(this);
    Process->start("/usr/local/bin/dmtcp_coordinator");
*/
    return 0;

}
