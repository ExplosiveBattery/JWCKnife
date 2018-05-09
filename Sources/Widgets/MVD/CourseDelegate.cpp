#include "CourseDelegate.h"
#include <QDebug>


//选课的delegate
CourseDelegate::CourseDelegate(QObject *parent) :
    QAbstractItemDelegate(parent)
{

}

void CourseDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QPixmap pixmap; int column =index.column();
    if(column==IFSEND_COLUMN) {
        if(index.model()->data(index).toBool())  pixmap.load(":/Resources/MainWindow/checkedHook.png");
        else pixmap.load(":/Resources/MainWindow/uncheckedHook.png");
    }else if(column==IFSTAR_COLUMN) {
        if(index.model()->data(index).toBool())  pixmap.load(":/Resources/MainWindow/checkedStar.png");
        else pixmap.load(":/Resources/MainWindow/uncheckedStar.png");
    }
    QApplication::style()->drawItemPixmap(painter, option.rect, Qt::AlignHCenter|Qt::AlignVCenter, pixmap );
}

QSize  CourseDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    return QSize(option.rect.width() ,option.rect.height());
}



bool CourseDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) {
    if ( event->type()==QEvent::MouseButtonRelease )  {
        int i1=1;
        bool returnValue=true;

        returnValue &= model->setData(index, model->data(index).toBool()?0:1);
        while(index.sibling(index.row()+i1,COURSEID_COLUMN).isValid() && index.sibling(index.row()+i1,CLASSID_COLUMN).isValid()
                && model->data(index.sibling(index.row()+i1,COURSEID_COLUMN))==model->data(index.sibling(index.row(),COURSEID_COLUMN))
                && model->data(index.sibling(index.row()+i1,CLASSID_COLUMN))==model->data(index.sibling(index.row(),CLASSID_COLUMN))
                ) {
            returnValue &= model->setData(index.sibling(index.row()+i1,index.column()), model->data(index).toBool());
            ++i1;
        }
        static_cast<QSqlTableModel*>(model)->submitAll();
        emit model->dataChanged(index,index.sibling(index.row()+i1,index.column()));

        if( returnValue==true )  return true;
        else return false;
    }
    return QAbstractItemDelegate::editorEvent(event, model, option, index);
}
