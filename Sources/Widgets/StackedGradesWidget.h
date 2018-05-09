#ifndef STACKEDGRADESWIDGET_H
#define STACKEDGRADESWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QFontDatabase>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QScrollArea>

#include <QSqlDatabase>
#include <QSqlQuery>
#include "MVD/CheckBoxHeader.h"
#include "MVD/EvaluationDelegate.h"
#include "MVD/AutoTurnLineDelegate.h"
#include <QTimer>
bool SQLErrorDetect (QSqlQuery *pQuery, const QString strCommand,const char *info);
void fillTableWidget(QTableWidget *tableWidget, QSqlQuery *pQuery, const QString strCommand,const char *info);

namespace Ui {
class StackedGradesWidget;
}

class StackedGradesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StackedGradesWidget(QWidget *parent = 0);
    ~StackedGradesWidget();
    void setUsername(QString username);
    void fillStackedWidget();
signals:
    void getGrades();
public slots:
    void handleStackButtonClick(int order);
    void handleHeaderCheckBoxClicked(int state);
    void flushGrades();
private slots:
    void on_pushButton_clicked();
    void on_pushButton_flush_clicked();
    void setUnaccessible();
private:
    QFont m_font;
    Ui::StackedGradesWidget *ui;
    QScrollArea *m_scrollArea;
    QStackedWidget *m_stackedWidget;
    EvaluationDelegate *m_evaluationDelegate;
    AutoTurnLineDelegate *m_tableDelegate;
    QVector<CheckBoxHeader *> m_vCheckboxHeader;
    int m_lastOrder;
    QStringList m_page0Semster,m_page1Semster;
    QSqlQuery *m_sqlQuery;
    int m_flush;QTimer *m_timer;
    QString m_username;

    //因为是使用了function.cpp中的填充函数，所以没有USERNAME_COLUMN
    enum columnGrade{IFCHECKED_COLUMN, SEMESTER_COLUMN, COURSEID_COLUMN, CLASSID_COLUMN, COURSENAME_COLUMN,CREDIT_COLUMN, ATTRIBUTE_COLUMN,GRADE_COLUMN, REASON_COLUMN};

};

#endif // STACKEDGRADESWIDGET_H
