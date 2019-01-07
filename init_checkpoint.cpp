#include "mainwindow.h"
#include "qcustomplot.h"
#include "ui_mainwindow.h"

int MainWindow::Init_CheckPoint() {

    QProcess *Process;

    Process = new QProcess(this);
    Process->start("/usr/local/bin/dmtcp_coordinator");

    return 0;

}
