#ifndef PLAYROLE_H
#define PLAYROLE_H

#include <QObject>
#include <QPixmap>
#include <QLabel>
#include "identity.h"
#include "card.h"
#include <vector>


class PlayRole:public QObject
{
    Q_OBJECT
public:
    PlayRole();
    int score=0;
    Identity identity;
    int Point;//Point的值就是相应的m_gameCell的数组的索引值，可取0——20
    int Point2=0;//finalCells数组的索引值,可取0——9
    QLabel playerlabel;
    bool isFinal=0;//是否进入决赛圈
    bool blocked=0;
    std::vector<Card> cards;//能力卡存储区
};
#endif // PLAYROLE_H
