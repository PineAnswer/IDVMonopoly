#ifndef GAMECELL_H
#define GAMECELL_H

#include <QString>
#include <QPoint>

enum class CellProperty {
    Normal,
    Start,
    Final,
    Card,
};

class GameCell {
public:
    GameCell(int m_id, const QPoint& m_position, const QString& m_imagePath, CellProperty m_property,int m_num);
    int id;
    QPoint position;
    QString imagePath;
    CellProperty property;
    int Num;
};

#endif // GAMECELL_H
