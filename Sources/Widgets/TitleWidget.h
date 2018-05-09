#ifndef TITLEWIDGET_H
#define TITLEWIDGET_H

//第二层导航栏，为了减小安装包的大小和方便自己，使用艺术字
//这个类的形式设计的代码没有编在视图中，而是在代码文件中
#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QStackedWidget>
#include <QFontDatabase>

class TitleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TitleWidget(QWidget *parent=0);
    void addButtons(QStringList slistComposition);
signals:
    void stackButtonClick(int order);
public slots:
    void handleStackButtonClick(void);

private:
    //display
    QHBoxLayout *m_mainLayout;
    QVector<QPushButton *> m_vButton;
    QFont m_font;
    //stacked widget
    int m_selectedStackButtonIndex;
};

#endif // TITLEWIDGET_H
