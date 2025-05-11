#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    chooserole=new ChooseRole;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_GameStart_clicked()
{
    this->close();
    chooserole->show();
}

