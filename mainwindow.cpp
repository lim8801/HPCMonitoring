#include "mainwindow.h"
#include "qcustomplot.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    printf("Start\n");

    Init_GPUInfo();
    printf("Init_GPUInfo_done\n");

    Init_ComboBox();
    printf("Init_ComboBox_done\n");

    Init_HostOverview();
    printf("Init_HostOverview_done\n");

    Init_DeviceOverview();
    printf("Init_DeviceOverview_done\n");

    Init_DevProcUsage();
    printf("Init_DevProcUsage_done\n");

    Init_DevMemUsage();
    printf("Init_DevMemUsage_done\n");

    Init_DevProperties();
    printf("Init_DevProperties_done\n");


    connect(ui->PU_Device_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(Reset_DevProcUsage()));
    connect(ui->PU_Type_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(Reset_DevProcUsage()));

    connect(ui->MU_Device_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(Reset_DevMemUsage()));

    connect(ui->Prop_Device_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(Reset_DevProperties()));

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(Checkpoint_Start_Coordinator()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(Checkpoint_Launch()));
    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(Checkpoint_Restart()));
    connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(Checkpoint_Setting()));
    connect(ui->pushButton_5, SIGNAL(clicked()), this, SLOT(Checkpoint_Reset()));
    connect(ui->pushButton_6, SIGNAL(clicked()), this, SLOT(Checkpoint_Close()));
    connect(ui->pushButton_7, SIGNAL(clicked()), this, SLOT(Checkpoint_Optimal()));
    connect(ui->pushButton_8, SIGNAL(clicked()), this, SLOT(visualizer_Launch()));

    connect(ui->pushButton_17, SIGNAL(clicked()), this, SLOT(FINISH()));

    connect(ui->pushButton_9, SIGNAL(clicked()), this, SLOT(BROWSE_1()));
    connect(ui->pushButton_10, SIGNAL(clicked()), this, SLOT(BROWSE_2()));
    connect(ui->pushButton_11, SIGNAL(clicked()), this, SLOT(BROWSE_3()));
    connect(ui->pushButton_12, SIGNAL(clicked()), this, SLOT(BROWSE_4()));

    perfLayout = new QFormLayout();
    //Qt::Alignment align = Qt::AlignVCenter;
    //perfLayout->setFormAlignment(align);
    QGroupBox *perfBox = new QGroupBox("Performance");
    perfBox->setLayout(perfLayout);

    powerLayout = new QFormLayout();
    QGroupBox *powerBox = new QGroupBox("Power");
    powerBox->setLayout(powerLayout);

    dataLayout = new QFormLayout();
    QGroupBox *dataBox = new QGroupBox("Data Access");
    dataBox->setLayout(dataLayout);

    topoLayout = new QFormLayout();
    QGroupBox *topoBox = new QGroupBox("Topology");
    topoBox->setLayout(topoLayout);

    QGroupBox *perfpowerBox = new QGroupBox();
    QVBoxLayout *perfpowerLayout = new QVBoxLayout();
    perfpowerLayout->addWidget(perfBox);
    perfpowerLayout->addWidget(powerBox);
    perfpowerBox->setLayout(perfpowerLayout);
    perfpowerBox->setFixedWidth(this->ui->Results->width()/2);

    QGroupBox *datatopoBox = new QGroupBox();
    QVBoxLayout *datatopoLayout = new QVBoxLayout();
    datatopoLayout->addWidget(dataBox);
    datatopoLayout->addWidget(topoBox);
    datatopoBox->setLayout(datatopoLayout);
    datatopoBox->setFixedWidth(this->ui->Results->width()/2);

    QHBoxLayout *totalLayout = new QHBoxLayout();
    totalLayout->addWidget(perfpowerBox);
    totalLayout->addWidget(datatopoBox);

    QGridLayout *baseLayout = new QGridLayout();
    baseLayout->addLayout(totalLayout, 0, 0);

    ui->Results->setLayout(baseLayout);

    /**/
    proc_list = new chkpt_process[10];
    for (int i = 0; i < 10; i++) {
        proc_list[i].latest_YY = 2017;
        proc_list[i].latest_MM = 11;
        proc_list[i].latest_DD = 7;
        proc_list[i].latest_h = 0;
        proc_list[i].latest_m = 0;
        proc_list[i].latest_s = i;
    }
    /**/

    //connect(ui->listWidget, SIGNAL(itemSelectionChanged()), this, SLOT(Reset_LtChkptDsp()));

    QTimer *dataTimer = new QTimer(this);
    connect(dataTimer, SIGNAL(timeout()), this, SLOT(Update_Window()));
    dataTimer->start(100);

}

MainWindow::~MainWindow()
{
    nvmlShutdown();

    delete[] HOVChart;

    delete[] DOVChart;
    delete[] DOVGrid;
    delete[] DOVDevice;

    delete[] SMChart;

    delete[] MemChart;

    delete ui;
}

void MainWindow::BROWSE_1()
{
    QFileDialog *fd = new QFileDialog(this);
    QString file_name;
    QStringList list_file_name;
    int ret = fd->exec();
    if (ret == (int)QDialog::Accepted)
    {
        list_file_name = fd->selectedFiles();
        file_name = list_file_name.at(0);
        ui->mpi_dir->setText(file_name);
    }
    else if (ret == (int)QDialog::Rejected)
    {
        QMessageBox::information(this, "", "File dir rejected!\n");
        return;
    }
    delete fd;
}

void MainWindow::BROWSE_2()
{
    QFileDialog *fd = new QFileDialog(this);
    QString file_name;
    QStringList list_file_name;
    int ret = fd->exec();
    if (ret == (int)QDialog::Accepted)
    {
        list_file_name = fd->selectedFiles();
        file_name = list_file_name.at(0);
        ui->tau_dir->setText(file_name);
    }
    else if (ret == (int)QDialog::Rejected)
    {
        QMessageBox::information(this, "", "File dir rejected!\n");
        return;
    }
    delete fd;
}

void MainWindow::BROWSE_3()
{
    QFileDialog *fd = new QFileDialog(this);
    QString file_name;
    QStringList list_file_name;
    int ret = fd->exec();
    if (ret == (int)QDialog::Accepted)
    {
        list_file_name = fd->selectedFiles();
        file_name = list_file_name.at(0);
        ui->smpi_dir->setText(file_name);
    }
    else if (ret == (int)QDialog::Rejected)
    {
        QMessageBox::information(this, "", "File dir rejected!\n");
        return;
    }
    delete fd;
}

void MainWindow::BROWSE_4()
{
    QFileDialog *fd = new QFileDialog(this);
    QString file_name;
    QStringList list_file_name;
    int ret = fd->exec();
    if (ret == (int)QDialog::Accepted)
    {
        list_file_name = fd->selectedFiles();
        file_name = list_file_name.at(0);
        ui->hw_info_dir->setText(file_name);
    }
    else if (ret == (int)QDialog::Rejected)
    {
        QMessageBox::information(this, "", "File dir rejected!\n");
        return;
    }
    delete fd;
}

void MainWindow::FINISH()
{
    system("/bin/ls");
    QFile *file = new QFile("tmp_result.txt");
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QTextStream *in = new QTextStream(file);

    for(int i = 0; i < perfLayout->rowCount(); i++)
        perfLayout->removeRow(i);

    while (!in->atEnd())
    {
        QString line = in->readLine();
        QLabel *label = new QLabel();
        perfLayout->addRow(line, label);
        delete label;
    }

    file->close();

    delete file;
    delete in;
}
