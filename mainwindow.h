#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "chooserole.h"
#include <playgame.h>
#include <QSoundEffect>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    ChooseRole* chooserole;
    QSoundEffect *bgMusic;

private slots:
    void paintEvent(QPaintEvent *event);

private:
    Ui::MainWindow *ui;
    void mousePressEvent(QMouseEvent *event);

    QLabel label;
};
#endif // MAINWINDOW_H
