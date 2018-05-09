#ifndef AUTOTURNLINEDELEGATE_H
#define AUTOTURNLINEDELEGATE_H
#include <QPainter>
#include <QStyledItemDelegate>
#include <QEvent>
#include <QSqlTableModel>



class AutoTurnLineDelegate : public QAbstractItemDelegate
{
public:
    AutoTurnLineDelegate(QObject *parent= Q_NULLPTR);
protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize  sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // AUTOTURNLINEDELEGATE_H
