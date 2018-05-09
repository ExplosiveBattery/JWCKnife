#include "EvaluationDelegate.h"

EvaluationDelegate::EvaluationDelegate(QObject *parent) :
    QAbstractItemDelegate(parent)
{

}


void EvaluationDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QPixmap pixmap; int column =index.column();
    if(column==IFSEND_COLUMN) {
        int judge =index.model()->data(index).toInt();
        if(judge==1)  pixmap.load(":/Resources/MainWindow/checkedHook.png");
        else if(judge==0) pixmap.load(":/Resources/MainWindow/uncheckedHook.png");
        else ;
    }
    QApplication::style()->drawItemPixmap(painter, option.rect, Qt::AlignHCenter|Qt::AlignVCenter, pixmap );
}

QSize  EvaluationDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    return QSize(option.rect.width() ,option.rect.height());
}



bool EvaluationDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) {
    if ( event->type()==QEvent::MouseButtonRelease )  {
        if( model->data(index).toInt()==2 ) return QAbstractItemDelegate::editorEvent(event, model, option, index);
        bool returnValue=true;

        returnValue &= model->setData(index, model->data(index).toBool()?0:1);
        static_cast<QSqlTableModel*>(model)->submit();
        emit model->dataChanged(index,index);

        if( returnValue==true )  return true;
        else return false;
    }
    return QAbstractItemDelegate::editorEvent(event, model, option, index);
}

