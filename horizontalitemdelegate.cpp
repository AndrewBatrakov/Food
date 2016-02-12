#include "horizontalitemdelegate.h"

HorizontalItemDelegate::HorizontalItemDelegate(QObject *parent, QString st) :
    QItemDelegate(parent)
{
    toSide = st;
}

void HorizontalItemDelegate::paint(QPainter *painter,
                             const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    //double tt = query.value(2).toReal();
    //editCost->setText(QString::number(tt,'f',2));
    QString text = QString::number(index.model()->data(index,Qt::DisplayRole).toReal(),'f',2);
    if(toSide == "left" || toSide == "center" || toSide == "right"){
        QStyleOptionViewItem myOption = option;
        if(toSide == "left"){
            myOption.displayAlignment = (Qt::AlignLeft | Qt::AlignVCenter);
            myOption.palette.setColor(QPalette::Text,Qt::darkGreen);
        }else if(toSide == "center"){
            myOption.displayAlignment = (Qt::AlignCenter | Qt::AlignVCenter);
            myOption.palette.setColor(QPalette::Text,Qt::darkBlue);
        }else if(toSide == "right"){
            myOption.displayAlignment = (Qt::AlignRight | Qt::AlignVCenter);
            myOption.palette.setColor(QPalette::Text,Qt::darkBlue);
        }
        drawDisplay(painter,myOption,myOption.rect,text);
    }else{
        QItemDelegate::paint(painter,option,index);
    }

}
