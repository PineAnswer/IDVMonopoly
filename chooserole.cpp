#include "chooserole.h"
#include <QPainter>
#include <QDebug>

ChooseRole::ChooseRole(QWidget *parent) : QWidget(parent) {
    //设置背景图片路径
    QString background_image_path = ":/resourse/image/backgroundImageChooseRole.png";
    QPixmap backgroundPixmap(background_image_path);
    if (!backgroundPixmap.isNull()) {
        this->setFixedSize(backgroundPixmap.size());
    }
    initializeUI();
    loadImages();
    statistic = new Statistics;
}

void ChooseRole::initializeUI() {
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(20);

    // 玩家标签和图片区域
    topLayout = new QGridLayout();
    topLayout->setHorizontalSpacing(30);
    topLayout->setVerticalSpacing(15);

    // 玩家按钮
    QString playerNames[] = {"玩家一", "玩家二", "玩家三"};
    for (int i = 0; i < 3; ++i) {
        //玩家按钮
        playerButtons[i] = new QPushButton(playerNames[i]);
        playerButtons[i]->setStyleSheet("text-align: center;");
        playerButtons[i]->setStyleSheet("font-size: 16px; font-weight: bold;");
        playerButtons[i]->setCursor(Qt::PointingHandCursor);
        playerButtons[i]->setFlat(true);
        topLayout->addWidget(playerButtons[i], 0, i);

        // 玩家图片按钮
        playerImageButtons[i] = new QPushButton();
        playerImageButtons[i]->setFixedSize(120, 120);
        playerImageButtons[i]->setStyleSheet("border: none;");
        topLayout->addWidget(playerImageButtons[i], 1, i);

        // 连接按钮点击信号
        connect(playerButtons[i], &QPushButton::clicked, [this, i]() { onPlayerButtonClicked(i); });
    }

    // 角色选择区域
    characterLayout = new QHBoxLayout();
    characterLayout->setSpacing(15);

    for (int i = 0; i < 7; ++i) {
        characterButtons[i] = new QPushButton();
        characterButtons[i]->setFixedSize(120, 140);
        characterButtons[i]->setStyleSheet("border: 2px solid #ddd; border-radius: 5px;");
        characterButtons[i]->setCursor(Qt::PointingHandCursor);
        characterLayout->addWidget(characterButtons[i]);

        connect(characterButtons[i], &QPushButton::clicked, [this, i]() { onCharacterImageClicked(characterButtons[i]); });
    }

    // 添加开始游戏按钮
    goToManorButton = new QPushButton("开始游戏", this);

    // 调整按钮大小
    goToManorButton->setFixedSize(200, 60);
    goToManorButton->setStyleSheet("font-size: 18px; padding: 10px 20px;");
    goToManorButton->setCursor(Qt::PointingHandCursor);
    connect(goToManorButton, &QPushButton::clicked, this, &ChooseRole::onGoToManorButtonClicked);
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(characterLayout);

    // 调整按钮位置
    mainLayout->addWidget(goToManorButton, 0, Qt::AlignBottom | Qt::AlignCenter);
}

void ChooseRole::loadImages() {
    QString characterImagePaths[] = {
        ":/role/resourse/image/novelist.png",":/role/resourse/image/kunchong.png", ":/role/resourse/image/journalist.png",
        ":/role/resourse/image/explorer.png", ":/images/character5.png", ":/images/character6.png",
        ":/images/character7.png"
    };

    for (int i = 0; i < 7; ++i) {
        QPixmap pixmap(characterImagePaths[i]);
        characterButtons[i]->setIcon(pixmap);
        characterButtons[i]->setIconSize(characterButtons[i]->size());
        QString style = QString("border: 2px solid #ddd; border-radius: 5px;");
        characterButtons[i]->setStyleSheet(style);
    }
}

//玩家按钮的槽函数
void ChooseRole::onPlayerButtonClicked(int playerIndex) {
    // 取消之前的高亮
    if (currentSelectedPlayer != -1) {
        playerImageButtons[currentSelectedPlayer]->setStyleSheet("border: none;");
    }

    // 应用新的高亮
    currentSelectedPlayer = playerIndex;
    playerImageButtons[playerIndex]->setStyleSheet(highlightStyle);
}

//角色图片按钮的槽函数
void ChooseRole::onCharacterImageClicked(QPushButton *button) {
    if (currentSelectedPlayer == -1) return;

    // 获取点击的角色图片
    QPixmap selectedPixmap = button->icon().pixmap(button->size());
    playerImageButtons[currentSelectedPlayer]->setIcon(selectedPixmap);


    //记录每个玩家对应的是哪个角色
    if(button==characterButtons[0]){
        this->statistic->role[currentSelectedPlayer].identity.role=Identity::novelist;
        this->statistic->role[currentSelectedPlayer].identity.KaTongPath=QString(":/role/resourse/image/NovelistKatong.png");
    }
    else if(button==characterButtons[1]){
        this->statistic->role[currentSelectedPlayer].identity.role=Identity::entomologist;
        this->statistic->role[currentSelectedPlayer].identity.KaTongPath=QString(":/role/resourse/image/kunchongKatong.png");
    }
    else if(button==characterButtons[2]){
        this->statistic->role[currentSelectedPlayer].identity.role=Identity::journalist;
        this->statistic->role[currentSelectedPlayer].identity.KaTongPath=QString(":/role/resourse/image/journalistKaTong.png");
    }
    else if(button==characterButtons[3]){
        this->statistic->role[currentSelectedPlayer].identity.role=Identity::explorer;
        this->statistic->role[currentSelectedPlayer].identity.KaTongPath=QString(":/role/resourse/image/explorerKatong.png");
    }

    // 设置玩家图片按钮的图标
    playerImageButtons[currentSelectedPlayer]->setIcon(selectedPixmap);
}

void ChooseRole::onGoToManorButtonClicked() {
    playgame= new PlayGame(nullptr,statistic);
    this->hide();
    playgame->show();
}
