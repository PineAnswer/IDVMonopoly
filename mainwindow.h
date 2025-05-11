#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "chooserole.h"
#include <playgame.h>

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

private slots:
    void on_GameStart_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
