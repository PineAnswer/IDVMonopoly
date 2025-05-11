#ifndef RAWSTONE_H
#define RAWSTONE_H

#include <QObject>
#include <QWidget>
#include <QString>
#include "identity.h"

namespace Ui {
class PlayGame;
}

class RawStone:public QWidget
{
    Q_OBJECT
public:
    RawStone();
    int GoldContent;
    Identity owner;
    QString imagePath;
};

#endif // RAWSTONE_H
