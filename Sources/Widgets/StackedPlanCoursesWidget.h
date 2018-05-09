#ifndef STACKEDPLANCOURSESWIDGET_H
#define STACKEDPLANCOURSESWIDGET_H

#include <QWidget>
#include "CardArrayWidget.h"

class StackedPlanCoursesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StackedPlanCoursesWidget(QWidget *parent = 0);
    void addButtons(QVector<int> vSplit, QStringList sContent);
    void setUsername(QString username);
signals:

public slots:
    void handleWidgetClose();
    void showTreeWidget(QString);

private:
    CardArrayWidget *m_cardArrayWidget;
    //tree widget
    TreeViewWidget *m_treeView;
    QSqlTableModel *m_model;
    TableToTreeProxyModel *m_proxyModel;
    QString m_username;

    enum columnPlanCourses{ATTRIBUTE_COLUMN, IFATTENDED_COLUMN, COURSEID_COLUMN, COURSENAME_COLUMN, CREDIR_COLUMN};



};

#endif // STACKEDPLANCOURSESWIDGET_H
