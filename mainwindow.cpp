#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QDesktopWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    pressing(false)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);//隐藏窗口标题
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent* e)
{
    pressing = true;
    ref_point = e->pos();
}

void MainWindow::mouseReleaseEvent(QMouseEvent* e)
{
    pressing = false;
}

void MainWindow::mouseMoveEvent(QMouseEvent* e)
{
    if(pressing)
    {
        move(e->globalPos() - ref_point);
        QDesktopWidget *desktop = QApplication::desktop();
        qDebug() << desktop->screenGeometry(desktop->screenNumber(this));
    }
}
