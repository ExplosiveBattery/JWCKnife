#ifndef CLASSSCHEDULEVIEW_H
#define CLASSSCHEDULEVIEW_H
#include <QAbstractItemView>
#include <QRegion>
#include <QMenu>
//#include <QPaintEvent>
#include <QPainter>


//这个类还需要很多虚函数来实现
class ClassScheduleView : public QAbstractItemView
{
    Q_OBJECT
private:

public:
    ClassScheduleView();
    //必须进行实现的几个纯虚函数
    QModelIndex indexAt(const QPoint &point) const;
    bool isIndexHidden(const QModelIndex &index) const;
    int horizontalOffset() const;
    int verticalOffset() const;
    QRect visualRect(const QModelIndex &index) const;
    QRegion visualRegionForSelection(const QItemSelection &selection) const;
    void setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags flags);
    void scrollTo(const QModelIndex &index, ScrollHint hint = EnsureVisible);
    QModelIndex moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers);

    void paintEvent(QPaintEvent *);

public slots:
    void customMenuRequested(QPoint pos);



};

#endif // CLASSSCHEDULEVIEW_H
