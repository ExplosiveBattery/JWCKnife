#ifndef EVALUATIONDELEGATE_H
#define EVALUATIONDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QApplication>
#include <QEvent>
#include <QSqlTableModel>


class EvaluationDelegate : public QAbstractItemDelegate
{
public:
    EvaluationDelegate(QObject *parent);

protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize  sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

private:
    const int IFSEND_COLUMN=2;
};

#endif // EVALUATIONDELEGATE_H
