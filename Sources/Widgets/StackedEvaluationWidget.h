#ifndef STACKEDEVALUATIONWIDGET_H
#define STACKEDEVALUATIONWIDGET_H

#include <QTextCodec>
#include "ui_StackedEvaluationWidget.h"
#include <QWidget>
#include <QSqlTableModel>
#include <QSqlQuery>
#include "MVD/EvaluationDelegate.h"
#include "MVD/AutoTurnLineDelegate.h"
#include "MVD/CheckBoxHeader.h"
#include <QTimer>


extern QTextCodec *gbk;
QString gbkURLCoding(QString str);
bool SQLErrorDetect (QSqlQuery *pQuery, const QString strCommand, const char *info);


namespace Ui {
class StackedEvaluationWidget;
}

class StackedEvaluationWidget : public QWidget
{
    Q_OBJECT

public:
    StackedEvaluationWidget(QWidget *parent = 0);
    ~StackedEvaluationWidget();
    void setSemester(QString semester);
    void setUsername(QString username);
    void init();
private:
    QString transferUserChoose(QString str);


signals:
    void sendEvaluateInfo(QStringList);
    void flushEvaluationInfo(bool);
private:
    Ui::StackedEvaluationWidget *ui;
    int m_lastOrder;
    QSqlTableModel *m_evaluationModel;
    QSqlQuery *m_sqlQuery;
    EvaluationDelegate *m_evaluationDelegate;
    AutoTurnLineDelegate *m_tableDelegate;
    CheckBoxHeader *m_checkboxHeader;
    int m_recordNumber;
    int m_flush;
    QTimer *m_timer;
    QString m_semester,m_username;
    enum columnEvaluation{SEMESTER_COLUMN, USERNAME_COLUMN, IFSEND_COLUMN, PERSON_COLUMN, TYPE_COLUMN, COURSENAME_COLUMN, WJBM_COLUMN, BPR_COLUMN, PGNR_COLUMN};
public slots:
    void handleHeaderCheckBoxClicked(int state);
    void handleDataChanged();
    void flushEvaluationInfo();
    void handleStackButtonClick(int order);
private slots:
    void on_pushButton_reset_clicked();
    void on_pushButton_send_clicked();
    void on_pushButton_flush_clicked();
    void setUnaccessible();
};

#endif // STACKEDEVALUATIONWIDGET_H
