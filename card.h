#ifndef CARD_H
#define CARD_H

#include <QObject>

class Card
{
public:
    enum cardName{
        None=0,
        DontMove,
        Decline,
        StillMe,
        PosExchange,
        Flash
    };
    Card(cardName name=None);
    cardName name=None;
};

#endif // CARD_H
