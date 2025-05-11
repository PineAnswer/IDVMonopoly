#include "gamecell.h"

GameCell::GameCell(int id, const QPoint& position, const QString& imagePath, CellProperty property,int m_num)
    : id(id), position(position), imagePath(imagePath), property(property) {
    Num=m_num;
}
