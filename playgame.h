#ifndef PLAYGAME_H
#define PLAYGAME_H

#include <QWidget>
#include <QPainter>
#include <QLabel>
#include <QPushButton>
#include "gamecell.h"
#include "statistics.h"
#include <QGridLayout>
namespace Ui {class PlayGame;}


class PlayGame : public QWidget
{
    Q_OBJECT

public:
    PlayGame(QWidget *parent = nullptr,Statistics* sta0 = nullptr); // 构造函数
    ~PlayGame();

    std::vector<GameCell> m_gameCells;
    std::vector<GameCell> finalCells;

    //加载地图界面
    void loadAndShowCells();
    void paintEvent(QPaintEvent *event);
    void setupMap();
    Statistics* sta;
    QPushButton go;
    QPoint map[22];
    QLabel scoreBoardOfRole1;
    QLabel scoreBoardOfRole2;
    QLabel scoreBoardOfRole3;
    QLabel noticeBoard;
    QPushButton targetButtons[3];

    QWidget cardWidget[3];
    QGridLayout cardBar[3];
    QPushButton cardButton[3][4];


    void forward(int steps, PlayRole &role);
    void forward2(int steps, PlayRole &role);
    void updateGameStatus(bool final,QString winner);
    void loadSkillImage();
    void getSkill(PlayRole &role);
    void useCards();
    void showTargetSelection(Card::cardName skillType);
    void selectTargetPlayer(int targetPlayer);
    void applySkillToTarget(int targetPlayer, Card::cardName skillType);
    void triggerCellEffect(int cellIndex, int playerIndex);
    void on_go_clicked();
    void wait(int time);
};



#endif
