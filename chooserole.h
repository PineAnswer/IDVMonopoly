#ifndef CHOOSEROLE_H
#define CHOOSEROLE_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QPainter>
#include "playgame.h"
#include "statistics.h"
#include <QSoundEffect>

class ChooseRole : public QWidget {
    Q_OBJECT

public:
    explicit ChooseRole(QWidget *parent = nullptr);
    Statistics* statistic;
    PlayGame* playgame;
    QSoundEffect *bgMusic;

private slots:
    void onPlayerButtonClicked(int playerIndex);//点击角色的反应
    void onCharacterImageClicked(QPushButton *button);//点击角色图片的反应
    void onGoToManorButtonClicked();//点击前往庄园按钮的反应

private:
    void initializeUI();
    void loadImages();

    QPushButton *playerButtons[3];//三个玩家按钮
    QPushButton *playerImageButtons[3];//玩家按钮下面的图片
    QPushButton *characterButtons[7];//七个角色按钮
    QPushButton *goToManorButton;//开始菜单按钮
    QVBoxLayout *mainLayout;
    QGridLayout *topLayout;
    QHBoxLayout *characterLayout;

    int currentSelectedPlayer = -1;
    const QString highlightStyle = "border: 3px solid red; border-radius: 5px;";

};

#endif // CHOOSEROLE_H
