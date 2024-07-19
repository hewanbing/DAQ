#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "myString.h"
#include "myData.h"
#include "IEEE754.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    handle=0;
    handleStatus=new QLabel("[Handle Status: Unknown]");
    VHSStatus=new QLabel("[VHS Status: Unknown]");
    ui->statusbar->addPermanentWidget(VHSStatus);
    ui->statusbar->addPermanentWidget(handleStatus);
    mVHS=nullptr;
    startTimer(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_5_clicked()
{
    QString name=ui->comboBox->currentText();
    if(handle==nullptr){
        if(name == QString::fromLocal8Bit("VMUSB")){
            handle = new VMUSB();
        }else if(name == QString::fromLocal8Bit("v2718")){
            handle = new v2718();
        }
    }else if(handle->getType()!=name){
        if(name == QString::fromLocal8Bit("VMUSB")){
            handle = new VMUSB();
        }else if(name == QString::fromLocal8Bit("v2718")){
            handle = new v2718();
        }
    }
    if(handle->checkConnection()){
        handleStatus->setText("[Handle Status: Connected]");
        if(mVHS==nullptr)mVHS=new VHS(handle);
        ui->label_13->setStyleSheet("QLabel{background:#87CEFA;}");
    }
}

void MainWindow::on_pushButton_6_clicked()
{
    myString base(ui->lineEdit->text().toLatin1());
    mVHS->setBase(base.getLong());
    if(mVHS->checkFirmware()){
        VHSStatus->setText("[VHS Status: Connected]");
        ui->label_15->setStyleSheet("QLabel{background:#87CEFA;}");
    }
}

void MainWindow::timerEvent(QTimerEvent*){
    if(mVHS==nullptr)return;
    if(! mVHS->IsConnected)return;
    mVHS->refresh();
    if(mVHS->IsTemperatureGood){
        ui->label_17->setStyleSheet("QLabel{background:#87CEFA;}");
        ui->lcdNumber->setPalette(Qt::blue);
    }else{
        ui->label_17->setStyleSheet("QLabel{background:#FF0000;}");
        ui->lcdNumber->setPalette(Qt::red);
    }
    if(mVHS->IsSupplyGood){
        ui->label_18->setStyleSheet("QLabel{background:#87CEFA;}");
    }else{
        ui->label_18->setStyleSheet("QLabel{background:#FF0000;}");
    }
    if(mVHS->IsModuleGood){
        ui->label_19->setStyleSheet("QLabel{background:#87CEFA;}");
    }else{
        ui->label_19->setStyleSheet("QLabel{background:#FF0000;}");
    }
    if(mVHS->IsNoRamp){
        ui->label_20->setStyleSheet("QLabel{background:#87CEFA;}");
    }else{
        ui->label_20->setStyleSheet("QLabel{background:#FF0000;}");
    }
    if(mVHS->IsNoSumError){
        ui->label_21->setStyleSheet("QLabel{background:#87CEFA;}");
    }else{
        ui->label_21->setStyleSheet("QLabel{background:#FF0000;}");
    }
    if(mVHS->IsCommandComplete){
        ui->label_22->setStyleSheet("QLabel{background:#87CEFA;}");
    }else{
        ui->label_22->setStyleSheet("QLabel{background:#FF0000;}");
    }
    if(mVHS->IsStop){
        ui->label_23->setStyleSheet("QLabel{background:#87CEFA;}");
    }else{
        ui->label_23->setStyleSheet("QLabel{background:#FF0000;}");
    }
    if(mVHS->IsAdjustment){
        ui->label_24->setStyleSheet("QLabel{background:#87CEFA;}");
    }else{
        ui->label_24->setStyleSheet("QLabel{background:#FF0000;}");
    }
    ui->progressBar->setValue(-1*mVHS->Voltage[0]);
    ui->progressBar_3->setValue(-1*mVHS->Voltage[1]);
    ui->progressBar_5->setValue(-1*mVHS->Voltage[2]);
    ui->progressBar_7->setValue(-1*mVHS->Voltage[3]);
    if(mVHS->IsChannelOn[0]){
        ui->pushButton->setText("ON");
        ui->pushButton->setStyleSheet("background-color:#87CEFA");
    }else{
        ui->pushButton->setText("OFF");
        ui->pushButton->setStyleSheet("background-color:#FF0000");
    }
    if(mVHS->IsChannelOn[1]){
        ui->pushButton_2->setText("ON");
        ui->pushButton_2->setStyleSheet("background-color:#87CEFA");
    }else{
        ui->pushButton_2->setText("OFF");
        ui->pushButton_2->setStyleSheet("background-color:#FF0000");
    }
    if(mVHS->IsChannelOn[2]){
        ui->pushButton_3->setText("ON");
        ui->pushButton_3->setStyleSheet("background-color:#87CEFA");
    }else{
        ui->pushButton_3->setText("OFF");
        ui->pushButton_3->setStyleSheet("background-color:#FF0000");
    }
    if(mVHS->IsChannelOn[3]){
        ui->pushButton_4->setText("ON");
        ui->pushButton_4->setStyleSheet("background-color:#87CEFA");
    }else{
        ui->pushButton_4->setText("OFF");
        ui->pushButton_4->setStyleSheet("background-color:#FF0000");
    }
    ui->progressBar->setValue(mVHS->Voltage[0]);
    ui->progressBar->setFormat("%v/"+ QString("%1").arg(mVHS->voltageSet[0]));
    ui->progressBar_3->setValue(mVHS->Voltage[1]);
    ui->progressBar_3->setFormat("%v/"+ QString("%1").arg(mVHS->voltageSet[1]));
    ui->progressBar_5->setValue(mVHS->Voltage[2]);
    ui->progressBar_5->setFormat("%v/"+ QString("%1").arg(mVHS->voltageSet[2]));
    ui->progressBar_7->setValue(mVHS->Voltage[3]);
    ui->progressBar_7->setFormat("%v/"+ QString("%1").arg(mVHS->voltageSet[3]));

    ui->progressBar_2->setValue(mVHS->Current[0]);
    ui->progressBar_4->setValue(mVHS->Current[1]);
    ui->progressBar_6->setValue(mVHS->Current[2]);
    ui->progressBar_8->setValue(mVHS->Current[3]);

    ui->lcdNumber->display(mVHS->temperature);
    if(ui->checkBox_5->isChecked())monitor();
}

void MainWindow::on_pushButton_clicked()
{
    mVHS->channelSwitch(0);
}

void MainWindow::on_doubleSpinBox_valueChanged(double arg1)
{
    mVHS->setVoltage(0,arg1);
}

void MainWindow::on_pushButton_2_clicked()
{
    mVHS->channelSwitch(1);
}
void MainWindow::on_pushButton_3_clicked()
{
    mVHS->channelSwitch(2);
}

void MainWindow::on_pushButton_4_clicked()
{
    mVHS->channelSwitch(3);
}

void MainWindow::on_doubleSpinBox_2_valueChanged(double arg1)
{
     mVHS->setVoltage(1,arg1);
}

void MainWindow::on_doubleSpinBox_3_valueChanged(double arg1)
{
     mVHS->setVoltage(2,arg1);
}

void MainWindow::on_doubleSpinBox_4_valueChanged(double arg1)
{
     mVHS->setVoltage(3,arg1);
}
void MainWindow::monitor(){
    myString st1( ui->lineEdit_2->text().toStdString().c_str());
    long address1 = st1.getLong();
    myString st2( ui->lineEdit_3->text().toStdString().c_str());
    long address2 = st2.getLong();
    long d1,d2;
    handle->getBit(0x29,address1,&d1,0,15,16);
    handle->getBit(0x29,address2,&d2,0,15,16);

    ui->lcdNumber_4->display(QString("%1").arg(int(d1),16,2,QLatin1Char('0')));
    ui->lcdNumber_3->display(QString("%1").arg(int(d2),16,2,QLatin1Char('0')));
    long d=d1*pow(2,16)+d2;
    ui->lineEdit_4->setText(QString("%1").arg(IEEE754::toFloat(d)));
    ui->lineEdit_5->setText(QString("%1").arg(int(d),16,16));
    ui->lineEdit_6->setText(QString("%1").arg(int(d),16,10));
}
