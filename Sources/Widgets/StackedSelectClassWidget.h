#ifndef STACKEDCLASSSCHEDULEWIDGET_H
#define STACKEDCLASSSCHEDULEWIDGET_H

#include "ui_StackedSelectClassWidget.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QTableView>
#include <QHeaderView>
#include <QSqlTableModel>
#include "MVD/AutoTurnLineDelegate.h"
#include "MVD/CourseDelegate.h"
#include "MVD/MaterialDelegate.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QScrollBar>
#include "ProfessionChooseForm.h"
#include <QPropertyAnimation>


bool SQLErrorDetect (QSqlQuery *pQuery, const QString strCommand, const char *info);

namespace Ui {
class StackedSelectClassWidget;
}


class StackedSelectClassWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StackedSelectClassWidget(QWidget *parent = 0);
    ~StackedSelectClassWidget();
    void setSemester(QString semester);
    void setUsername(QString username);
    void moveMaterialSendButton();
    void setTable();
private:
    void mergeCells(void);
    bool judgeConflict(QString str0, QString str1);
signals:
    void haveAddedCourse(int,int,QString);
    void getAllCourses();
    void getSchoolPublicCourses();
    void getMaterialInfo();
    void sendMaterialInfo();

public slots:
    void handleStackButtonClick(int);
    void handleChoice(QString text);
    void handleAddCourse(int wantBegin, int wantDay);
    void handleSchoolPublicCoursesError(QString);
    void flushSchoolPublicCourses();
    void flushAllCourses();
    void flushMaterialInfo();
    void flushingAllCourses();
    void on_pushButton_prev_clicked();
    void on_pushButton_next_clicked();
    void on_pushButton_goto_clicked();
    void on_pushButton_profession_clicked();
    void on_pushButton_find_clicked();
    void on_pushButton_materials_send_clicked();
    void on_pushButton_addCourse_clicked();
    void on_pushButton_flush_clicked();
    void on_pushButton_flush_material_clicked();

private:
    enum columnCourse{SEMESTER_COLUMN,ISSCHOOLPUBLICCOURSE_COLUMN, IFCHOICE_COLUMN, IFSTAR_COLUMN,
               PROFESSION_COLUMN, COURSEID_COLUMN, COURSENAME_COLUMN, CLASSID_COLUMN,
               CREDIR_COLUMN, TYPEOFEXAM_COLUMN, TEACHER_COLUMN, CLASSREMAINING_COLUMN, CLASSLIMIT_COLUMN,
               TEACHINGWEEK_COLUMN, TEACHERDAY_COLUMN, TEACHINGSESSION_COLUMN, CAMPUS_COLUMN, TEACHINGBUILDING_COLUMN, TEACHINGROOM_COLUMN};
    enum columnMaterial{SEMESTER_MATERIAL_COLUMN, USERNAME_MATERIAL_COLUMN, IFCHOOSE_MATERIAL_COLUMN,  COURSENAME_MATERIAL_COLUMN, INFO_MATERIAL_COLUMN};
    Ui::StackedSelectClassWidget *ui;
    QVBoxLayout *m_mainLayout;
    QSqlTableModel *m_classModel, *m_materialModel;
    QTableView *m_tableView;
    AutoTurnLineDelegate *m_tableDelegate;
    CourseDelegate *m_courseDelegate;
    MaterialDelegate *m_materialDelegate;
    QWidget *m_professionChooseForm;
    QPropertyAnimation *m_stackAnimation;
    const int LIMIT =60; //数据库于每一个页面的显示条数的限制
    int m_lastOrder; //记录上一次的页栈index
    int m_page0,m_page1,m_page2;  //记录当前page0、page1已经翻到哪一页, 从0开始计数
    int m_scrollvalue0,m_scrollvalue1,m_scrollvalue3;  //记录换页时0,1,3 三个页面的垂直滚动条的滚动位置
    bool m_click0, m_click1;
    int ALLPAGENUMBER,PUBLICPAGENUMBER; //记录总页数
    QString m_semester,m_username;
};

#endif // STACKEDCLASSSCHEDULEWIDGET_H
