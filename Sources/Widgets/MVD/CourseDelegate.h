#ifndef CHECKBOXDELEGATE_H
#define CHECKBOXDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QApplication>
#include <QEvent>
#include <QSqlTableModel>

class CourseDelegate : public QAbstractItemDelegate
{

public:
    CourseDelegate(QObject *parent);
protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize  sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

private:
    const int IFSEND_COLUMN=2;
    const int IFSTAR_COLUMN=3;
    const int COURSEID_COLUMN =5;
    const int CLASSID_COLUMN =7;
};

#endif // CHECKBOXDELEGATE_H
