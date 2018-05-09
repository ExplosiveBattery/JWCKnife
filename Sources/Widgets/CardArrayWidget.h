#ifndef CARDARRAYWIDGET_H
#define CARDARRAYWIDGET_H

#include <QSqlTableModel>
#include <QTreeView>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QColor>
#include <QFontDatabase>
#include <QTime>
#include "MVD/TableToTreeProxyModel.h"
#include "TreeViewWidget.h"

bool SQLErrorDetect(QSqlQuery *pQuery, const QString strCommand, const char *info);

class CardArrayWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CardArrayWidget(QWidget *parent = 0);
    void addButtons(QVector<int> vSplit, QStringList sContent);

signals:
    void click(QString);
public slots:
    void handleButtonClick();

public:
    //display
    QVBoxLayout *m_mainLayout;
    QVector<QPushButton *> m_vButton;
    QFont m_font;
    //unused
    //QVector<QColor> m_vColor ={QColor(64,224,208),QColor(255,105,180),QColor(65,105,225),QColor(245,222,179),QColor(240,128,128),QColor(214,181,188),QColor(240,214,189),QColor(255,230,215),QColor(222,230,206),QColor(222,230,206),QColor(222,230,206),QColor(247,247,197),QColor(214,230,181),QColor(115,189,140),QColor(190,221,223),QColor(222,188,140),QColor(230,165,189),QColor(247,173,174),QColor(230,188,189)};
};

#endif // CARDARRAYWIDGET_H4
