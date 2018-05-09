#include "MaterialDelegate.h"
MaterialDelegate::MaterialDelegate(QObject *parent) :
    QAbstractItemDelegate(parent)
{

}



QSize  MaterialDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    return QSize(option.rect.width() ,option.rect.height());
}


void MaterialDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    if( index.column()==IFCHOOSE_COLUMN ) {
        QStyleOptionButton radioButtonOption;
        radioButtonOption.rect =QRect(20,option.rect.y(), 30,30);
        radioButtonOption.state |= QStyle::State_Enabled;
        radioButtonOption.text ="是";
        if(index.model()->data(index).toBool()) radioButtonOption.state |= QStyle::State_On;
        else radioButtonOption.state |= QStyle::State_Off;
        QApplication::style()->drawControl(QStyle::CE_RadioButton, &radioButtonOption, painter);


        radioButtonOption.state =QStyle::State_Enabled;
        radioButtonOption.text ="否";
        radioButtonOption.rect =QRect(70,option.rect.y(), 30,30);
        if(index.model()->data(index).toBool()) radioButtonOption.state |= QStyle::State_Off;
        else radioButtonOption.state |= QStyle::State_On;
        QApplication::style()->drawControl(QStyle::CE_RadioButton, &radioButtonOption, painter);

    }

}

bool MaterialDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) {
    QMouseEvent* e =(QMouseEvent*)event;
    int returnValue =true;
    if( index.column()==IFCHOOSE_COLUMN ) {

        if( e->x()>=20 && e->x()<=50 ) returnValue &= model->setData(index, 1);
        else if( e->x()>=70 && e->x()<=100 ) returnValue &= model->setData(index, 0);
        else ;
        model->submit();
        emit model->dataChanged(index,index);
    }
    return returnValue;
}
