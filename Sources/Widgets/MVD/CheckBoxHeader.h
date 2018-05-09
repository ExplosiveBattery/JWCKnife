#ifndef CHECKBOXHEADER_H
#define CHECKBOXHEADER_H

#include <QHeaderView>
#include <QPainter>

class CheckBoxHeader : public QHeaderView
{
    Q_OBJECT

public:
    CheckBoxHeader(Qt::Orientation orientation, QWidget* parent = 0);
    int getCheckState() const;
    void setCheckState(int state);

signals:
    void checkBoxClicked(int state);

protected:
    void paintSection(QPainter* painter, const QRect& rect, int index) const;
    void mousePressEvent(QMouseEvent* event);

private:
    /* 0表示空，1表示半选，2表示勾选 */
    int m_checkState;

};

#endif // CHECKBOXHEADER_H
