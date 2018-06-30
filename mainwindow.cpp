#include "incrementaldatatransfer.h"
#include "mainwindow.h"
#include "mockiodevice.h"
#include "ui_mainwindow.h"

#include <QFile>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto src = new MockIODevice(1100, 3, 32*1024, true);
    auto dest = new MockIODevice(1500, 4, 64*1024, false);
    new IncrementalDataTransfer(src, dest, this);

    src = new MockIODevice(1500, 3, 32*1024, false);
    dest = new MockIODevice(600, 4, 64*1024, false);
    new IncrementalDataTransfer(src, dest, this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
