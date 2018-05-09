#ifndef STACKEDEXAMRELATEDWIDGET_H
#define STACKEDEXAMRELATEDWIDGET_H

#include "ui_StackedExamRelatedWidget.h"
#include <QMessageBox>
#include <QWidget>
#include <QVBoxLayout>
#include "TitleWidget.h"
#include <QPropertyAnimation>
#include <QSqlTableModel>
#include <QSqlQuery>
#include "MVD/AutoTurnLineDelegate.h"
#include <QTimer>

bool SQLErrorDetect (QSqlQuery *pQuery, const QString strCommand,const char *info);

namespace Ui {
class StackedExamRelatedWidget;
}
class StackedExamRelatedWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StackedExamRelatedWidget(QWidget *parent = 0);
    ~StackedExamRelatedWidget();
    void setSemester(QString semester);
    void setUsername(QString username);
    void init(void);
signals:
    void getExamInfo(bool);
public slots:
    void handleStackButtonClick(int);
    void displayResult(const QString &);
    void flushExamInfo();
private slots:
    void on_pushButton_flush_clicked();
    void setUnaccessible();
private:
    Ui::StackedExamRelatedWidget *ui;
    QPropertyAnimation *m_stackAnimation;
    int m_lastOrder,m_flush;
    QSqlTableModel *m_ExamModel;
    AutoTurnLineDelegate *m_tableDelegate;
    QSqlQuery *m_sqlQuery;
    QTimer *m_timer;
    QString m_semester,m_username;

    enum columnExam{SEMESTER_COLUMN, USERNAME_COLUMN, ATTRIBUTE_COLUMN, CAMPUS_COLUMN, BUILDING_COLUMN, ROOM_COLUMN, COURSENAME_COLUMN, WEEK_COLUMN, DAY_COLUMN, DATE_COLUMN, TIME_COLUMN, POSITIONID_COLUMN};
};

#endif // STACKEDEXAMRELATEDWIDGET_H
