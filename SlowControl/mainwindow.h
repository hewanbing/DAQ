#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "vmehandle.h"
#include "vmusb.h"
#include "QLabel"
#include "vhs.h"
#include <QTimerEvent>
#include "v2718.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void timerEvent(QTimerEvent*);
    void monitor();

private slots:
    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_clicked();

    void on_doubleSpinBox_valueChanged(double arg1);

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_doubleSpinBox_2_valueChanged(double arg1);

    void on_doubleSpinBox_3_valueChanged(double arg1);

    void on_doubleSpinBox_4_valueChanged(double arg1);

private:
    Ui::MainWindow *ui;
    VMEHandle *handle;
    QLabel *handleStatus;
    QLabel *VHSStatus;
    VHS* mVHS;

};
#endif // MAINWINDOW_H
