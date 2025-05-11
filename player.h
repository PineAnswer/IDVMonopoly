#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QWidget>

#include "identity.h"

class Player
{
    Q_OBJECT
public:
    Player();
    Identity job;
};

#endif // PLAYER_H
