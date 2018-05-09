#ifndef RADIOBUTTONDELEGATE_H
#define RADIOBUTTONDELEGATE_H


#include <QApplication>
#include <QStyledItemDelegate>
#include <QMouseEvent>


class MaterialDelegate  : public QAbstractItemDelegate
{

protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize  sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

public:
    MaterialDelegate(QObject *parent);
    const int IFCHOOSE_COLUMN =2;
};

#endif // RADIOBUTTONDELEGATE_H
