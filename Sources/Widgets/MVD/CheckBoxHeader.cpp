#include "CheckBoxHeader.h"

CheckBoxHeader::CheckBoxHeader(Qt::Orientation orientation, QWidget* parent /*= 0*/)
    : QHeaderView(orientation, parent)
{
    m_checkState = 0;
}



void CheckBoxHeader::paintSection(QPainter* painter, const QRect& rect, int index) const
{
    painter->save();
    QHeaderView::paintSection(painter, rect, index);
    painter->restore();
    if (index == 2) {
        QStyleOptionButton option;
        option.rect = QRect(13,7,20,20);
        option.state = QStyle::State_Enabled | QStyle::State_Active;
        switch ( m_checkState ) {
            case 0://没有任何选择
                option.state |= QStyle::State_Off;
                break;
            case 1://部分选择，显示为半选状态
                option.state |= QStyle::State_NoChange;
                break;
            case 2://全部选择，显示为勾选状态
                option.state |= QStyle::State_On;
                break;
            default:
                break;
        }
        style()->drawPrimitive(QStyle::PE_IndicatorCheckBox, &option, painter);
    }
}

void CheckBoxHeader::mousePressEvent(QMouseEvent* event)
{
    if(m_checkState!=2) {
        m_checkState =2;
    }else {
        m_checkState =0;
    }
    viewport()->update();
    emit checkBoxClicked(m_checkState);
}

int CheckBoxHeader::getCheckState() const {
    return m_checkState;
}

//对外（parent）提供的接口
void CheckBoxHeader::setCheckState(int state) {
    m_checkState =state;
    viewport()->update();
}
