#ifndef STATISTICS_H
#define STATISTICS_H

#include "playrole.h"

class Statistics
{
public:
    Statistics();
    //玩家数据存放
    PlayRole role[3];
    enum Turn{
        ATurn,BTurn,CTurn
    };
    Turn turn=ATurn;
    int steps;
    int round=0;
    int currentSkillUser=-1;
    Card::cardName currentSkillType;
};

#endif // STATISTICS_H
