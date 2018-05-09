#include "AutoTurnLineDelegate.h"

AutoTurnLineDelegate::AutoTurnLineDelegate(QObject *parent) :QAbstractItemDelegate(parent)
{

}


void AutoTurnLineDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                                 const QModelIndex &index) const
{
    QString text = index.model()->data(index, Qt::DisplayRole).toString();
    painter->drawText( option.rect, Qt::TextWordWrap | Qt::AlignHCenter | Qt::AlignVCenter, text );
}

QSize  AutoTurnLineDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QFont font( option.font );
    const QFontMetrics fm( font );
    const QString text=index.model()->data(index).toString();
    int width = fm.width(text);
    int height = fm.height();
    return QSize(width ,height);
}
