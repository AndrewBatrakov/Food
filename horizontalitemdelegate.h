#ifndef HORIZONTALITEMDELEGATE_H
#define HORIZONTALITEMDELEGATE_H

#include <QAbstractItemModel>
#include <QtWidgets>

class HorizontalItemDelegate : public QItemDelegate
{

public:
    HorizontalItemDelegate(QObject *parent,QString);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
private:
    QString toSide;
};

#endif // HORIZONTALITEMDELEGATE_H
