#include "playgame.h"
#include <QLabel>
#include <QRandomGenerator>
#include "playrole.h"
#include "qeventloop.h"
#include "statistics.h"
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QTimer>
#include <QEventLoop>


PlayGame::PlayGame(QWidget *parent,Statistics* sta0): QWidget(parent)
{
    //设置界面基本格式
    {
    this->setFixedSize(1200,800);
    QString background_image_path = ":/resourse/image/qipanbackground.png";
    QPixmap backgroundPixmap(background_image_path);
    backgroundPixmap = QPixmap(background_image_path);
    }
    //掷骰子按键相关
    {
    go.setParent(this);
    go.move(550,670);
    go.setFixedSize(100,120);
    go.setIcon(QPixmap(":/resourse/image/dice.png"));
    go.setStyleSheet(
        "QPushButton {"
        "    border: none;"           // 移除边框
        "    background-color: none;" // 透明背景
        "    icon-size: 100px 100px;"  // 设置图标大小为按钮大小
        "    padding: 5px;"
        "}"
        );
    go.show();
    }

    //计分板初始化
    {
    QString scoreShow1="玩家一破译进度：0";
    QString scoreShow2="玩家二破译进度：0";
    QString scoreShow3="玩家三破译进度：0";
    scoreBoardOfRole1.setParent(this);
    scoreBoardOfRole1.move(952,116);
    scoreBoardOfRole1.setStyleSheet("color: white; font-size: 24px;");
    scoreBoardOfRole1.setText(scoreShow1);
    scoreBoardOfRole2.setParent(this);
    scoreBoardOfRole2.move(952,344);
    scoreBoardOfRole2.setStyleSheet("color: white; font-size: 24px;");
    scoreBoardOfRole2.setText(scoreShow2);
    scoreBoardOfRole3.setParent(this);
    scoreBoardOfRole3.move(952,530);
    scoreBoardOfRole3.setStyleSheet("color: white; font-size: 24px;");
    scoreBoardOfRole3.setText(scoreShow3);
    }

    //能力栏初始化
    {
    for(int i=0;i<3;i++){
        cardWidget[i].setParent(this);
        cardWidget[i].setFixedSize(135,135);
        cardWidget[i].setLayout(&cardBar[i]);
        cardBar[i].setParent(&cardWidget[i]);
        cardBar[i].setContentsMargins(1, 1, 1, 1); // 网格内边距
        cardBar[i].setSpacing(5); // 网格间距
        for(int j=0;j<4;j++){
            cardButton[i][j].setFixedSize(65,65);
            cardButton[i][j].setParent(&cardWidget[i]);
            cardBar[i].addWidget(&cardButton[i][j],j/2,j%2);
            cardButton[i][j].setStyleSheet(
                "QPushButton {"
                "    border: none;"           // 移除边框
                "    background-color: none;" // 透明背景
                "    icon-size: 65px 65px;"  // 设置图标大小为按钮大小
                "    padding: 5px;"
                "}"
                );
            cardButton[i][j].setEnabled(false);
            cardButton[i][j].setIcon(QPixmap(":/role/resourse/image/Anywhere.png"));
        }
    }
    cardWidget[0].setGeometry(1010, 170, 135, 135);
    cardWidget[1].setGeometry(1010, 390, 135, 135);
    cardWidget[2].setGeometry(1010, 590, 135, 135);
    }

    //提示板初始化
    {
    QString notice="请玩家一掷骰子！";
    noticeBoard.setParent(this);
    noticeBoard.setStyleSheet("color: white; font-size: 40px; font-family: 'Segoe UI', sans-serif; font-weight: bold;");
    noticeBoard.move(0,35);
    noticeBoard.setAlignment(Qt::AlignCenter);
    noticeBoard.setFixedSize(1200, 40);
    noticeBoard.setText(notice);
    }

    setupMap();
    loadAndShowCells();
    sta=sta0;
    loadSkillImage();

    //人物状态初始化
    {
    for(int i=0;i<3;i++){
        sta->role[i].playerlabel.setParent(this);
    }
    sta->role[0].Point=19;
    sta->role[0].playerlabel.move(m_gameCells[19].position);
    sta->role[0].playerlabel.setScaledContents(true);
    sta->role[0].playerlabel.setFixedSize(50,90);
    sta->role[0].playerlabel.setPixmap(sta->role[0].identity.KaTongPath);
    sta->role[0].playerlabel.show();

    sta->role[1].Point=5;
    sta->role[1].playerlabel.move(m_gameCells[5].position);
    sta->role[1].playerlabel.setScaledContents(true);
    sta->role[1].playerlabel.setFixedSize(50,90);
    sta->role[1].playerlabel.setPixmap(sta->role[1].identity.KaTongPath);
    sta->role[1].playerlabel.show();

    sta->role[2].Point=12;
    sta->role[2].playerlabel.move(m_gameCells[12].position);
    sta->role[2].playerlabel.setScaledContents(true);
    sta->role[2].playerlabel.setFixedSize(50,90);
    sta->role[2].playerlabel.setPixmap(sta->role[2].identity.KaTongPath);
    sta->role[2].playerlabel.show();
    }

    //技能释放对象按钮
    for (int k = 0; k < 3; ++k) {
        targetButtons[k].setParent(this);
        targetButtons[k].hide();  // 默认隐藏
        connect(&targetButtons[k], &QPushButton::clicked, this, [this, k]() {
            int targetPlayer = targetButtons[k].text().right(1).toInt() - 1;
            applySkillToTarget(targetPlayer, sta->currentSkillType);
        });
    }

    //链接槽函数
    connect(&go, &QPushButton::clicked, this, &PlayGame::on_go_clicked);
    for(int i=0;i<3;i++){
        for(int j=0;j<4;j++){
            connect(&cardButton[i][j],&QPushButton::clicked,this,&PlayGame::useCards);
        }
    }
}

void PlayGame::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    QString background_image_path = ":/resourse/image/qipanbackground.png"; // 替换为实际的背景图片路径
    QPixmap backgroundPixmap(background_image_path);
    if (!backgroundPixmap.isNull()) { // 检查图片是否加载成功
        // 将背景图片缩放至窗口大小并绘制
        painter.drawPixmap(0, 0, backgroundPixmap.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    }
}

void PlayGame::useCards() {
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    QString notice;
    if (!button) return;
    // 遍历查找按钮在二维数组中的位置
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (&cardButton[i][j] == button) {
                switch(sta->role[i].cards[j].name) {
                    case Card::None:
                        break;
                    case Card::DontMove:
                        sta->currentSkillUser = i;
                        showTargetSelection(Card::DontMove);
                        break;
                    case Card::Decline:
                        sta->currentSkillUser = i;
                        showTargetSelection(Card::Decline);
                        break;
                    case Card::StillMe:
                        sta->turn = static_cast<Statistics::Turn>(i);
                        notice = "玩家" + QString::number(i+1) + "获得额外回合！";
                        noticeBoard.setText(notice);
                        if(i==0){
                            sta->round--;
                        }
                        sta->role[i].cards[j]=Card::None;
                        for(int j=0;j<4;j++){
                            cardButton[i][j].setEnabled(false);
                        }
                        loadSkillImage();
                        return;
                    case Card::PosExchange:
                        sta->currentSkillUser = i;
                        showTargetSelection(Card::PosExchange);
                        break;
                    case Card::Flash:{
                        int randomPos = QRandomGenerator::global()->bounded(0, 21);
                        PlayRole& role = sta->role[i];
                        //int oldPos = role.Point;
                        role.Point = randomPos;
                        role.playerlabel.move(m_gameCells[randomPos].position);
                        triggerCellEffect(randomPos, i);
                        noticeBoard.setText("玩家" + QString::number(i+1) + "闪现了");
                        wait(1000);
                        break;
                    }
                }
                sta->role[i].cards[j]=Card::None;
                cardButton[i][j].setEnabled(false);
                loadSkillImage();
                updateGameStatus(false, "");
                return;
                }
            }
        }
    }

void PlayGame::on_go_clicked(){
    go.setEnabled(false);
    int steps = QRandomGenerator::global()->bounded(1, 5);     //范围：[1，4）
    QString notice;
    switch(this->sta->turn){
    case Statistics::ATurn:
        sta->round++;
        //如果是第一圈
        if(sta->role[0].blocked){
            noticeBoard.setText("玩家一该回合被剥夺行动资格！");
            wait(1000);
            sta->role[0].blocked = false;
            sta->turn = Statistics::BTurn;
            noticeBoard.setText("请玩家二掷骰子！");
            go.setEnabled(true);
            return;
        }
        if(!sta->role[0].isFinal){
            for(int j=0;j<4;j++){
                cardButton[2][j].setEnabled(false);
            }
            notice="玩家一掷出了"+QString::number(steps);
            noticeBoard.setText(notice);
            wait(1000);
            this->forward(steps,this->sta->role[0]);

            sta->role[0].score += m_gameCells[sta->role[0].Point].Num;//加分

            if(sta->round%2==0)
                getSkill(sta->role[0]);
            if(m_gameCells[sta->role[0].Point].property==CellProperty::Card)
                getSkill(sta->role[0]);//获得技能卡

            sta->turn = Statistics::BTurn;
            updateGameStatus(0,"");
            for(int i=0;i<4;i++){
                if(sta->role[0].cards[i].name!=Card::None)
                    cardButton[0][i].setEnabled(true);
            }
            return;
        }
        //如果是第二圈
            notice="玩家一掷出了"+QString::number(steps);
            noticeBoard.setText(notice);
            this->forward2(steps,this->sta->role[0]);
            sta->turn = Statistics::BTurn;
            updateGameStatus(sta->role[0].Point2==9?1:0,"玩家一");
            return;


    case Statistics::BTurn:
        //如果是第一圈
        if(sta->role[1].blocked){
            noticeBoard.setText("玩家二该回合被剥夺行动资格！请玩家三掷骰子！");
            wait(1000);
            sta->role[1].blocked = false;
            sta->turn = Statistics::CTurn;
            go.setEnabled(true);
            return;
        }
        if(!sta->role[1].isFinal){
            for(int i=0;i<4;i++){
                cardButton[0][i].setEnabled(false);
            }
            notice="玩家二掷出了"+QString::number(steps);
            noticeBoard.setText(notice);
            wait(1000);
            this->forward(steps,this->sta->role[1]);
            sta->role[1].score += m_gameCells[sta->role[1].Point].Num;
            //如果轮次为偶数
            if(sta->round%2==0)
                getSkill(sta->role[1]);
            //如果走到了领取能力卡这一格
            if(m_gameCells[sta->role[1].Point].property==CellProperty::Card)
                getSkill(sta->role[1]);
            sta->turn = Statistics::CTurn;
            updateGameStatus(0,"");
            for(int i=0;i<4;i++){
                if(sta->role[1].cards[i].name!=Card::None)
                    cardButton[1][i].setEnabled(true);
            }
            return;
        }
        //如果是第二圈
        notice="玩家二掷出了"+QString::number(steps);
        noticeBoard.setText(notice);
        this->forward2(steps,this->sta->role[1]);
        sta->turn = Statistics::CTurn;
        updateGameStatus(sta->role[1].Point2==9?1:0,"玩家二");
        return;

    case Statistics::CTurn:
        //如果是第一圈
        if(sta->role[2].blocked){
            noticeBoard.setText("玩家三该回合被剥夺行动资格！请玩家一掷骰子！");
            sta->role[2].blocked = false;
            sta->turn = Statistics::ATurn;
            go.setEnabled(true);
            return;
        }
        if(!sta->role[2].isFinal){
            for(int i=0;i<4;i++){
                cardButton[1][i].setEnabled(false);
            }

            notice="玩家三掷出了"+QString::number(steps);
            noticeBoard.setText(notice);
            wait(1000);
            this->forward(steps,this->sta->role[2]);

            sta->role[2].score += m_gameCells[sta->role[2].Point].Num;
            if(sta->round%2==0)
                getSkill(sta->role[2]);
            if(m_gameCells[sta->role[2].Point].property==CellProperty::Card)
                getSkill(sta->role[2]);
            sta->turn = Statistics::ATurn;
            updateGameStatus(0,"");

            for(int i=0;i<4;i++){
                if(sta->role[2].cards[i].name!=Card::None)
                    cardButton[2][i].setEnabled(true);
            }
            return;
        }
        //如果是第二圈
        notice="玩家三掷出了"+QString::number(steps);
        noticeBoard.setText(notice);
        this->forward2(steps,this->sta->role[2]);
        sta->turn = Statistics::ATurn;
        updateGameStatus(sta->role[0].Point2==9?1:0,"玩家三");
        return;
    }
}

void PlayGame::forward(int steps, PlayRole &role){
    //前进动画部分
    QSequentialAnimationGroup *group = new QSequentialAnimationGroup(this);
    for(int i=0;i<steps;i++){
        QPropertyAnimation* anim = new QPropertyAnimation(&role.playerlabel, "pos");
        anim->setDuration(300);
        anim->setStartValue(m_gameCells[role.Point].position);
        role.Point = (role.Point + 1) % 21;
        anim->setEndValue(m_gameCells[role.Point].position);
        anim->setEasingCurve(QEasingCurve::OutQuad);
        group->addAnimation(anim);
        if(m_gameCells[role.Point].property==CellProperty::Start&&role.score>=100)   {
            role.isFinal=1;
            break;
        }
    }
    group->start(QAbstractAnimation::DeleteWhenStopped);
    if(m_gameCells[role.Point].property==CellProperty::Start&&role.score>=100){
        role.playerlabel.move(finalCells[0].position);
        role.isFinal=1;
    }
}

void PlayGame::forward2(int steps, PlayRole &role){
    QSequentialAnimationGroup *group = new QSequentialAnimationGroup(this);
    for(int i=0;i<steps;i++){
        QPropertyAnimation* anim = new QPropertyAnimation(&role.playerlabel, "pos");
        anim->setDuration(300);
        anim->setStartValue(finalCells[role.Point2++].position);
        anim->setEndValue(finalCells[role.Point2].position);
        anim->setEasingCurve(QEasingCurve::OutQuad);
        group->addAnimation(anim);
    }


    group->start(QAbstractAnimation::DeleteWhenStopped);
}

void PlayGame::updateGameStatus(bool end, QString winner) {
    go.setEnabled(true);
    QString notice;
    //已经有人到达终点
    if(end){
        notice="游戏结束！"+winner+"获胜！";
        return;
    }
    //否则
    //1
    switch(this->sta->turn) {
    case Statistics::ATurn: notice = "玩家三可使用技能！玩家一可掷骰子！"; break;
    case Statistics::BTurn: notice = "玩家一可使用技能！玩家二可掷骰子！"; break;
    case Statistics::CTurn: notice = "玩家二可使用技能！玩家三可掷骰子！"; break;
    }
    noticeBoard.setText(notice);
    //2
    QString scoreShow1 = "玩家一破译进度：" + QString::number(sta->role[0].score);
    scoreBoardOfRole1.setText(scoreShow1);
    scoreBoardOfRole1.adjustSize();
    QString scoreShow2 = "玩家二目前进度：" + QString::number(sta->role[1].score);
    scoreBoardOfRole2.setText(scoreShow2);
    scoreBoardOfRole2.adjustSize();
    QString scoreShow3 = "玩家三目前进度：" + QString::number(sta->role[2].score);
    scoreBoardOfRole3.setText(scoreShow3);
    scoreBoardOfRole3.adjustSize();

    //加载技能图片
    this->loadSkillImage();
}

void PlayGame::getSkill(PlayRole &role){
    bool isFull=true;
    int randomCard=QRandomGenerator::global()->bounded(1, 6);
    for(int i=0;i<4;i++){
        if(role.cards[i].name==Card::None){
            role.cards[i]=Card::cardName(randomCard);
            isFull=false;
            break;
        }
    }
    if(isFull)role.cards[3]=Card::cardName(randomCard);
    noticeBoard.setText("玩家"+QString::number(&role-sta->role+1)+"获得了一张能力卡！");
    wait(1000);

}

void PlayGame::loadSkillImage(){
    for(int i=0;i<3;i++){
        for(int j=0;j<4;j++){
            switch(sta->role[i].cards[j].name){
            case Card::None:
                cardButton[i][j].setIcon(QPixmap(":/role/resourse/image/None.png"));
                break;
            case Card::DontMove:
                cardButton[i][j].setIcon(QPixmap(":/role/resourse/image/DontMove.png"));
                break;
            case Card::Decline:
                cardButton[i][j].setIcon(QPixmap(":/role/resourse/image/Decline.png"));
                break;
            case Card::StillMe:
                cardButton[i][j].setIcon(QPixmap(":/role/resourse/image/StillMe.png"));
                break;
            case Card::PosExchange:
                cardButton[i][j].setIcon(QPixmap(":/role/resourse/image/PosExchange.png"));
                break;
            case Card::Flash:
                cardButton[i][j].setIcon(QPixmap(":/role/resourse/image/Flash.png"));
                break;
            }
    }
}
}

void PlayGame::showTargetSelection(Card::cardName skillType) {
    sta->currentSkillType = skillType;
    for (int k = 0; k < 3; ++k) {
        if (k != sta->currentSkillUser) {  // 排除自己
            targetButtons[k].setText("选择玩家" + QString::number(k+1));
            targetButtons[k].setGeometry(400, 300 + k*50, 200, 40);
            targetButtons[k].show();
        }
    }
    QString skillName;
    switch(skillType) {
    case Card::DontMove:
        skillName = "封禁";
        break;
    case Card::Decline:
        skillName = "失常";
        break;
    case Card::PosExchange:
        skillName = "换位";
        break;
    default:
        break;
    }
    noticeBoard.setText("请选择要" + skillName + "的玩家");
}

void PlayGame::applySkillToTarget(int targetPlayer, Card::cardName skillType) {
    for (int k = 0; k < 3; ++k) {
        targetButtons[k].hide();
    }

    switch(skillType) {
    case Card::DontMove:
        sta->role[targetPlayer].blocked = true;
        noticeBoard.setText("玩家" + QString::number(sta->currentSkillUser+1) +
                            "禁止了玩家" + QString::number(targetPlayer+1) + "下一回合移动！");
        break;

    case Card::Decline:
        if (sta->role[targetPlayer].score >= 15) {
            sta->role[targetPlayer].score -= 15;
        }
        else {
            sta->role[targetPlayer].score = 0;
        }
        noticeBoard.setText("玩家" + QString::number(sta->currentSkillUser+1) +
                            "降低了玩家" + QString::number(targetPlayer+1) + "的破译进度！");
        break;

    case Card::PosExchange:
        std::swap(sta->role[sta->currentSkillUser].Point, sta->role[targetPlayer].Point);
        sta->role[sta->currentSkillUser].playerlabel.move(m_gameCells[sta->role[sta->currentSkillUser].Point].position);
        sta->role[targetPlayer].playerlabel.move(m_gameCells[sta->role[targetPlayer].Point].position);
        noticeBoard.setText("玩家" + QString::number(sta->currentSkillUser+1) +
                            "与玩家" + QString::number(targetPlayer+1) + "交换了位置！");
        break;

    case Card::None:
    case Card::StillMe:
    case Card::Flash:
        break;
    }
    wait(1000);
    // 恢复游戏状态
    updateGameStatus(false, "");
}

void PlayGame::triggerCellEffect(int cellIndex, int playerIndex) {
    QEventLoop loop;
    GameCell& targetCell = m_gameCells[cellIndex];
    PlayRole& player = sta->role[playerIndex];

    switch(targetCell.property) {

    case CellProperty::Normal:
        player.score += targetCell.Num;
        break;

    case CellProperty::Card:
        getSkill(player);
        break;

    case CellProperty::Start:
        if (player.score >= 10 && !player.isFinal) {
            player.isFinal = true;
            player.playerlabel.move(finalCells[0].position);
            noticeBoard.setText("玩家" + QString::number(playerIndex+1) + "进入最终阶段！");
            QTimer::singleShot(1000, &loop, &QEventLoop::quit);
            loop.exec();
        }
        break;

    case CellProperty::Final:
        break;
    }
}

void PlayGame::setupMap(){
    m_gameCells.emplace_back(1, QPoint(340, 123), ":/resourse/image/kuangshi.png", CellProperty::Normal,6);
    m_gameCells.emplace_back(2, QPoint(196, 157), ":/resourse/image/kuangshi.png", CellProperty::Normal,10);
    m_gameCells.emplace_back(3, QPoint(98, 251), ":/resourse/image/kuangshi.png", CellProperty::Normal,8);
    m_gameCells.emplace_back(4, QPoint(172, 353), ":/resourse/image/CardCell.png", CellProperty::Card,0);
    m_gameCells.emplace_back(5, QPoint(105, 459), ":/resourse/image/kuangshi.png", CellProperty::Normal,4);
    m_gameCells.emplace_back(6, QPoint(150, 562), ":/resourse/image/startpoint.png", CellProperty::Start,0);
    m_gameCells.emplace_back(7, QPoint(253, 617), ":/resourse/image/kuangshi.png", CellProperty::Normal,10);
    m_gameCells.emplace_back(8, QPoint(336, 586), ":/resourse/image/kuangshi.png", CellProperty::Normal,4);
    m_gameCells.emplace_back(9, QPoint(410, 621), ":/resourse/image/CardCell.png", CellProperty::Card,0);
    m_gameCells.emplace_back(10, QPoint(509, 565), ":/resourse/image/kuangshi.png", CellProperty::Normal,6);
    m_gameCells.emplace_back(11, QPoint(626, 615), ":/resourse/image/kuangshi.png", CellProperty::Normal,8);
    m_gameCells.emplace_back(12, QPoint(737, 601), ":/resourse/image/kuangshi.png", CellProperty::Normal,12);
    m_gameCells.emplace_back(13, QPoint(833, 623), ":/resourse/image/startpoint.png", CellProperty::Start,0);
    m_gameCells.emplace_back(14, QPoint(930, 560), ":/resourse/image/kuangshi.png", CellProperty::Normal,6);
    m_gameCells.emplace_back(15, QPoint(890, 475), ":/resourse/image/kuangshi.png", CellProperty::Normal,8);
    m_gameCells.emplace_back(16, QPoint(880, 370), ":/resourse/image/CardCell.png", CellProperty::Card,0);
    m_gameCells.emplace_back(17, QPoint(944, 265), ":/resourse/image/kuangshi.png", CellProperty::Normal,10);
    m_gameCells.emplace_back(18, QPoint(900, 144), ":/resourse/image/kuangshi.png", CellProperty::Normal,4);
    m_gameCells.emplace_back(19, QPoint(732, 158), ":/resourse/image/kuangshi.png", CellProperty::Normal,12);
    m_gameCells.emplace_back(20, QPoint(617, 123), ":/resourse/image/startpoint.png", CellProperty::Start,0);
    m_gameCells.emplace_back(21, QPoint(478, 145), ":/resourse/image/kuangshi.png", CellProperty::Normal,12);

    finalCells.emplace_back(1,QPoint(364,488),":/role/resourse/image/finalCell.png",CellProperty::Final,0);
    finalCells.emplace_back(2,QPoint(285,411),":/role/resourse/image/finalCell.png",CellProperty::Final,0);
    finalCells.emplace_back(3,QPoint(336,328),":/role/resourse/image/finalCell.png",CellProperty::Final,0);
    finalCells.emplace_back(4,QPoint(401,262),":/role/resourse/image/finalCell.png",CellProperty::Final,0);
    finalCells.emplace_back(5,QPoint(520,240),":/role/resourse/image/finalCell.png",CellProperty::Final,0);
    finalCells.emplace_back(6,QPoint(606,263),":/role/resourse/image/finalCell.png",CellProperty::Final,0);
    finalCells.emplace_back(7,QPoint(693,315),":/role/resourse/image/finalCell.png",CellProperty::Final,0);
    finalCells.emplace_back(8,QPoint(782,391),":/role/resourse/image/finalCell.png",CellProperty::Final,0);
    finalCells.emplace_back(9,QPoint(697,472),":/role/resourse/image/finalCell.png",CellProperty::Final,0);
    finalCells.emplace_back(10,QPoint(566,498),":/role/resourse/image/door.png",CellProperty::Final,0);
}

void PlayGame::loadAndShowCells(){
    for (const auto& cell : m_gameCells) {
        QPixmap pixmap(cell.imagePath);
        if (!pixmap.isNull()) {
            QLabel *cellLabel1 = new QLabel(this);
            cellLabel1->setPixmap(pixmap.scaled(80, 80, Qt::KeepAspectRatio));
            cellLabel1->move(cell.position);
            cellLabel1->show();
        }
    }
    for (const auto& cell : m_gameCells) {
        if (cell.property==CellProperty::Normal) {
            QLabel *cellLabel2 = new QLabel(this);
            cellLabel2->setText(QString::number(cell.Num));
            cellLabel2->move(cell.position.x()+15,cell.position.y()+10);
            QFont font("Arial", 30, QFont::Bold, false);  // 字体名称、大小、粗细、斜体
            cellLabel2->setFont(font);
            cellLabel2->show();
        }
    }
    for (const auto& cell : finalCells) {
        QPixmap pixmap(cell.imagePath);
        if (!pixmap.isNull()) {
            QLabel *cellLabel = new QLabel(this);
            cellLabel->setPixmap(pixmap.scaled(80, 80, Qt::KeepAspectRatio));
            cellLabel->move(cell.position);
            cellLabel->show();
        }
    }
}

void PlayGame::wait(int time){
    QEventLoop loop;
    QTimer::singleShot(time, &loop, &QEventLoop::quit);
    loop.exec();
}

PlayGame::~PlayGame(){

}
