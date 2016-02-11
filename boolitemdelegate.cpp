#include "boolitemdelegate.h"

BoolItemDelegate::BoolItemDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

void BoolItemDelegate::paint(QPainter *painter,
                             const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    bool stateBase = index.model()->data(index,Qt::DisplayRole).toBool();
    if(stateBase == 0 || stateBase == 1){
        QString text;
        //Qt::CheckState  state;
        QStyleOptionViewItem myOption = option;
        myOption.displayAlignment = Qt::AlignCenter;
        if(stateBase == true){
            text = tr("Server");
            myOption.palette.setColor(QPalette::Text,Qt::darkGreen);
        }else{
            text = tr("Local");
            myOption.palette.setColor(QPalette::Text,Qt::darkBlue);
        }
        drawDisplay(painter,myOption,myOption.rect,text);
    }else{
        QItemDelegate::paint(painter,option,index);
    }

}

//void BoolItemDelegate::drawCheck(QPainter *painter, const QStyleOptionViewItem &option,
//                                 const QRect &rect, Qt::CheckState state) const
//{
//    const int textMargin = QApplication::style()->pixelMetric(QStyle::PM_FocusFrameHMargin) + 1;
//    QRect checkRect = QStyle::alignedRect(option.direction, Qt::AlignCenter,rect.size(),
//                                          QRect(option.rect.x() + textMargin, option.rect.y(),
//                                                option.rect.width() - (textMargin * 2),
//                                                option.rect.height()));
//    BoolItemDelegate::drawCheck(painter,option,rect,state);
//}


//    QStyleOptionViewItemV4 opt = QStyleOptionViewItemV4(option);
//    initStyleOption(&opt,index);
//    const QStyleOptionViewItemV4 *v4 = qstyleoption_cast<const QStyleOptionViewItemV4 *>(&option);
//    const QWidget *widget = v4->widget;
//    QStyle *style = widget ? widget->style() : QApplication::style();
//    //style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);
//    painter->save();
//    int leftTab = 100;
//    QRect leftRect(option.rect.x() + 5, option.rect.y() + 5, leftTab, option.rect.height());
//    style->drawItemText(painter, leftRect, Qt::AlignLeft, v4->palette, v4->state & QStyle::State_Enabled, "This picture", QPalette::WindowText);

//    QPixmap pix(":/add.png");
//    pix.scaled(QSize(20,20),Qt::KeepAspectRatio);
//    QRect backgroundRect(option.rect.x() + leftTab + 5, option.rect.y() + 5, 18, 18);
//    style->drawItemPixmap(painter, backgroundRect, Qt::AlignCenter, pix);

//    leftTab += 16;

//    QRect rightRect(option.rect.x() + leftTab + 10, option.rect.y() + 5, option.rect.width() - leftTab, option.rect.height());

//    style->drawItemText(painter, rightRect, Qt::AlignLeft, v4->palette, v4->state & QStyle::State_Enabled, "painted by QStyledItemDelegate", QPalette::WindowText);
//    painter->restore();
