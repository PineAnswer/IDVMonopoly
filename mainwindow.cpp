#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QPixmap>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    chooserole=new ChooseRole;
    QString background_image_path = ":/resourse/image/mainsceneBG.jpg";
    QPixmap backgroundPixmap(background_image_path);
    if (!backgroundPixmap.isNull()) {
        this->setFixedSize(backgroundPixmap.size());
    }
    //设置背景音乐
    bgMusic = new QSoundEffect(this);
    bgMusic->setSource(QUrl::fromLocalFile(":/resourse/music/mainsceneBgMusic.wav"));
    bgMusic->setLoopCount(QSoundEffect::Infinite); // 无限循环
    bgMusic->setVolume(0.5f); // 音量范围 0.0-1.0
    bgMusic->play();

}
void MainWindow::paintEvent(QPaintEvent *event)
{
    QPixmap background_image_path = QString(":/resourse/image/mainsceneBG.jpg");
    QPixmap backgroundPixmap(background_image_path);
    if (!backgroundPixmap.isNull()) {
        QPainter painter(this);
        painter.drawPixmap(0, 0, backgroundPixmap.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    }
    QMainWindow::paintEvent(event);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    this->close();
    chooserole->show();
    chooserole->bgMusic=this->bgMusic;
    QMainWindow::mousePressEvent(event); // 调用基类实现，确保默认行为
}

MainWindow::~MainWindow()
{
    delete ui;
}



