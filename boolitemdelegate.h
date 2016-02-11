#ifndef BOOLITEMDELEGATE_H
#define BOOLITEMDELEGATE_H

#include <QAbstractItemModel>
#include <QtWidgets>

class BoolItemDelegate : public QItemDelegate
{

public:
    BoolItemDelegate(QObject *parent);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
//    virtual void drawCheck(QPainter *painter, const QStyleOptionViewItem &option,
//                           const QRect &rect, Qt::CheckState state) const;
};

#endif // BOOLITEMDELEGATE_H
