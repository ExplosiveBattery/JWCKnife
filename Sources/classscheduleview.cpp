#include "classscheduleview.h"
#include <QDebug>

ClassScheduleView::ClassScheduleView()
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(customMenuRequested(QPoint)));
}


void ClassScheduleView::paintEvent(QPaintEvent *) {
    QPainter painter(this);
//    painter.setBrush(QColor(155,144,194));
    painter.setPen(QColor(155,144,194));

}


//需要通过坐标判断出是否是在已经有课的区域中，不同的区域进行不同的显示
void ClassScheduleView::customMenuRequested(QPoint pos) {
//    QModelIndex index=indexAt(pos);
    QMenu *menu=new QMenu(this);
	menu->addAction(new QAction("增加课程", this));
	menu->addAction(new QAction("Action 2", this));
	menu->addAction(new QAction("Action 3", this));
    menu->popup(viewport()->mapToGlobal(pos));
}
