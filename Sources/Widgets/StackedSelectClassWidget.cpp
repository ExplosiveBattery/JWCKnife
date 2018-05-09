#include "StackedSelectClassWidget.h"
#include <QMessageBox>
#include <QDebug>
StackedSelectClassWidget::StackedSelectClassWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::StackedSelectClassWidget), m_mainLayout(new QVBoxLayout(this)),m_professionChooseForm(new ProfessionChooseForm),
    m_classModel(new QSqlTableModel(this)), m_materialModel(new QSqlTableModel(this)), m_tableDelegate(new AutoTurnLineDelegate(this)), m_courseDelegate(new CourseDelegate(this)), m_materialDelegate(new MaterialDelegate(this)),
    m_page0(0),m_page1(0),m_lastOrder(0),m_scrollvalue0(0),m_scrollvalue1(0),m_scrollvalue3(0),m_click0(0),m_click1(0),
    m_stackAnimation(new QPropertyAnimation(this,"geometry"))
{
    ui->setupUi(this);
    ui->widget_title->addButtons(QStringList{"全部课程", "校公选课程",  "自由选择",  "课程收藏页",  "教材自定义",  "大物实验选课"});
    connect(ui->widget_title, SIGNAL(stackButtonClick(int)), this, SLOT(handleStackButtonClick(int)) );
    connect(m_professionChooseForm, SIGNAL(choose(QString)), this, SLOT(handleChoice(QString)));
    m_stackAnimation->setTargetObject(ui->stackedWidget);
    m_stackAnimation->setDuration(200);

    setTable();
}

StackedSelectClassWidget::~StackedSelectClassWidget()
{
    delete ui;
}

//“教材”功能中，根据QTableView的记录数量来动态设置“确认”按钮
void StackedSelectClassWidget::moveMaterialSendButton()
{
    QSqlQuery *sqlQuery =new QSqlQuery(QSqlDatabase::database());
    QPoint pos = ui->pushButton_materials_send->pos();
    SQLErrorDetect(sqlQuery, "select count(*) from Materials where Semester='"+m_semester+"' and Username='"+m_username+"'", "获取所有教材数量失败 in StackedSelectClassWidget::setTable()");
    sqlQuery->next();
    int recordNumber =sqlQuery->value(0).toInt();
    if(recordNumber<12) {  //原大小只能刚好装下12条记录
        pos.setY( (recordNumber+1)*30+ui->tableView_materials->y()+10 );
        ui->pushButton_materials_send->move(pos);
    }else {
        pos.setY( (12+1)*30+ui->tableView_materials->y()+10 );
        ui->pushButton_materials_send->move(pos);
    }
    delete sqlQuery;
}

void StackedSelectClassWidget::setSemester(QString semester)
{
    m_semester =semester;
}


void StackedSelectClassWidget::setUsername(QString username)
{
    m_username =username;
}


void StackedSelectClassWidget::setTable(){
    //设置页数
    QSqlQuery *sqlQuery =new QSqlQuery(QSqlDatabase::database());
    SQLErrorDetect(sqlQuery, "select count(*) from Courses where Semester='"+m_semester+"';", "获取所有开课数量失败 in StackedSelectClassWidget::setTable()");
    sqlQuery->next();
    int recordNumber =sqlQuery->value(0).toInt();
    ALLPAGENUMBER=(recordNumber==recordNumber/LIMIT*LIMIT&&recordNumber!=0)?recordNumber/LIMIT:recordNumber/LIMIT+1;
    SQLErrorDetect(sqlQuery, "select count(*) from Courses where IsSchoolPublicCourse=1 and Semester='"+m_semester+"';", "获取校公选开课数量失败 in StackedSelectClassWidget::setTable()");
    sqlQuery->next();recordNumber =sqlQuery->value(0).toInt();
    PUBLICPAGENUMBER=(PUBLICPAGENUMBER==recordNumber/LIMIT*LIMIT&&recordNumber!=0)?recordNumber/LIMIT:recordNumber/LIMIT+1;
    delete sqlQuery;
    ui->label_page->setText("/"+QString::number(ALLPAGENUMBER)+"页");

    m_classModel->setTable("Courses");
    m_classModel->setFilter(" Semester='"+m_semester+"' limit "+QString::number(LIMIT));
    m_classModel->select();
    m_classModel->setEditStrategy(QSqlTableModel::OnManualSubmit); //希望使用submitAll函数 提交内容
    m_classModel->setHeaderData(IFCHOICE_COLUMN, Qt::Horizontal,  "选择");
    m_classModel->setHeaderData(IFSTAR_COLUMN, Qt::Horizontal,  "收藏");
    m_classModel->setHeaderData(PROFESSION_COLUMN, Qt::Horizontal,  "开课系");
    m_classModel->setHeaderData(COURSEID_COLUMN, Qt::Horizontal,  "课程号");
    m_classModel->setHeaderData(COURSENAME_COLUMN, Qt::Horizontal,  "课程名字");
    m_classModel->setHeaderData(CLASSID_COLUMN, Qt::Horizontal,  "课序号");
    m_classModel->setHeaderData(CREDIR_COLUMN, Qt::Horizontal,  "学分");
    m_classModel->setHeaderData(TYPEOFEXAM_COLUMN, Qt::Horizontal,  "考试类型");
    m_classModel->setHeaderData(TEACHER_COLUMN, Qt::Horizontal,  "教师");
    m_classModel->setHeaderData(CLASSREMAINING_COLUMN, Qt::Horizontal,  "课余量");
    m_classModel->setHeaderData(CLASSLIMIT_COLUMN, Qt::Horizontal,  "选课限制");
    m_classModel->setHeaderData(TEACHINGWEEK_COLUMN, Qt::Horizontal,  "周次");
    m_classModel->setHeaderData(TEACHERDAY_COLUMN, Qt::Horizontal,  "星期");
    m_classModel->setHeaderData(TEACHINGSESSION_COLUMN, Qt::Horizontal,  "节次");
    m_classModel->setHeaderData(CAMPUS_COLUMN, Qt::Horizontal,  "校区");
    m_classModel->setHeaderData(TEACHINGBUILDING_COLUMN, Qt::Horizontal,  "教学楼");
    m_classModel->setHeaderData(TEACHINGROOM_COLUMN, Qt::Horizontal,  "教室");
    ui->tableView_courses->setModel(m_classModel);
    ui->tableView_courses->hideColumn(SEMESTER_COLUMN); //隐藏Semester
    ui->tableView_courses->hideColumn(ISSCHOOLPUBLICCOURSE_COLUMN); // 隐藏IsSchoolPublicCourse
    ui->tableView_courses->resizeColumnsToContents();
    ui->tableView_courses->setColumnWidth(IFCHOICE_COLUMN, 50);
    ui->tableView_courses->setColumnWidth(IFSTAR_COLUMN, 50);
    ui->tableView_courses->setColumnWidth(PROFESSION_COLUMN, 80);
    ui->tableView_courses->setColumnWidth(COURSEID_COLUMN, 80);
    ui->tableView_courses->setColumnWidth(COURSENAME_COLUMN, 140);
    ui->tableView_courses->setColumnWidth(CLASSID_COLUMN, 50);
    ui->tableView_courses->setColumnWidth(CREDIR_COLUMN, 40);
    ui->tableView_courses->setColumnWidth(TYPEOFEXAM_COLUMN, 60);
    ui->tableView_courses->setColumnWidth(TEACHER_COLUMN, 80);
    ui->tableView_courses->setColumnWidth(CLASSREMAINING_COLUMN, 50);
    ui->tableView_courses->setColumnWidth(CLASSLIMIT_COLUMN, 80);
    ui->tableView_courses->setColumnWidth(TEACHINGWEEK_COLUMN, 90);
    ui->tableView_courses->setColumnWidth(TEACHERDAY_COLUMN, 30);
    ui->tableView_courses->setColumnWidth(TEACHINGSESSION_COLUMN, 50);
    ui->tableView_courses->setColumnWidth(CAMPUS_COLUMN, 50);
    ui->tableView_courses->setColumnWidth(TEACHINGBUILDING_COLUMN, 80);
    ui->tableView_courses->setColumnWidth(TEACHINGROOM_COLUMN, 100);
    //ui->tableView_courses->verticalHeader()->setVisible(false); 这个会导致左边边界出现异常
    for(int i1=PROFESSION_COLUMN;i1 <= TEACHINGROOM_COLUMN;++i1)
        ui->tableView_courses->setItemDelegateForColumn(i1, m_tableDelegate);
    ui->tableView_courses->setItemDelegateForColumn(IFCHOICE_COLUMN, m_courseDelegate);//IfSend字段 管理hook图标
    ui->tableView_courses->setItemDelegateForColumn(IFSTAR_COLUMN, m_courseDelegate); //IfStar字段 管理star图标
    mergeCells();

    m_materialModel->setTable("Materials");
    m_materialModel->setFilter(" Semester='"+m_semester+"' and Username='"+m_username+"'");
    m_materialModel->select();
    m_materialModel->setHeaderData(IFCHOOSE_MATERIAL_COLUMN, Qt::Horizontal,  "教材订购选项");
    m_materialModel->setHeaderData(COURSENAME_MATERIAL_COLUMN, Qt::Horizontal,  "课程名字");
    ui->tableView_materials->setModel(m_materialModel);
    ui->tableView_materials->hideColumn(SEMESTER_MATERIAL_COLUMN);
    ui->tableView_materials->hideColumn(USERNAME_MATERIAL_COLUMN);
    ui->tableView_materials->hideColumn(INFO_MATERIAL_COLUMN);
    ui->tableView_materials->setColumnWidth(IFCHOOSE_MATERIAL_COLUMN,120);
    ui->tableView_materials->setColumnWidth(COURSENAME_MATERIAL_COLUMN,350);
    ui->tableView_materials->setItemDelegateForColumn(IFCHOOSE_MATERIAL_COLUMN, m_materialDelegate);
    ui->tableView_materials->setItemDelegateForColumn(COURSENAME_MATERIAL_COLUMN, m_tableDelegate);
}

void StackedSelectClassWidget::mergeCells(void) {
    int rowCount =1;
    QString lastCourseId,lastClassId;
    QSqlRecord reocrd =m_classModel->record(0);
    lastCourseId =reocrd.value("CourseId").toString();
    lastClassId =reocrd.value("ClassId").toString();
    int allCount =m_classModel->rowCount();

    for(int i1=1;i1<allCount;++i1) {

        reocrd =m_classModel->record(i1);
        if( lastCourseId==reocrd.value("CourseId").toString() && lastClassId==reocrd.value("ClassId").toString()) {
            ++rowCount;
            if(i1==allCount-1) {
                ui->tableView_courses->setSpan(i1-rowCount+1,1,rowCount,1);
                ui->tableView_courses->setSpan(i1-rowCount+1,2,rowCount,1);
                ui->tableView_courses->setSpan(i1-rowCount+1,3,rowCount,1);
                ui->tableView_courses->setSpan(i1-rowCount+1,4,rowCount,1);
                ui->tableView_courses->setSpan(i1-rowCount+1,5,rowCount,1);
                ui->tableView_courses->setSpan(i1-rowCount+1,6,rowCount,1);
                ui->tableView_courses->setSpan(i1-rowCount+1,7,rowCount,1);
                ui->tableView_courses->setSpan(i1-rowCount+1,8,rowCount,1);
                ui->tableView_courses->setSpan(i1-rowCount+1,9,rowCount,1);
                ui->tableView_courses->setSpan(i1-rowCount+1,10,rowCount,1);
                ui->tableView_courses->setSpan(i1-rowCount+1,11,rowCount,1);
           }
        }else {
            if(rowCount!=1) {
                ui->tableView_courses->setSpan(i1-rowCount,1,rowCount,1);
                ui->tableView_courses->setSpan(i1-rowCount,2,rowCount,1);
                ui->tableView_courses->setSpan(i1-rowCount,3,rowCount,1);
                ui->tableView_courses->setSpan(i1-rowCount,4,rowCount,1);
                ui->tableView_courses->setSpan(i1-rowCount,5,rowCount,1);
                ui->tableView_courses->setSpan(i1-rowCount,6,rowCount,1);
                ui->tableView_courses->setSpan(i1-rowCount,7,rowCount,1);
                ui->tableView_courses->setSpan(i1-rowCount,8,rowCount,1);
                ui->tableView_courses->setSpan(i1-rowCount,9,rowCount,1);
                ui->tableView_courses->setSpan(i1-rowCount,10,rowCount,1);
                ui->tableView_courses->setSpan(i1-rowCount,11,rowCount,1);
                rowCount =1;
            }
            lastCourseId =reocrd.value("CourseId").toString();
            lastClassId =reocrd.value("ClassId").toString();
        }
    }
}

void StackedSelectClassWidget::handleStackButtonClick(int order){
    if( m_lastOrder!=order ) {
        if( order<3 ) {
            m_stackAnimation->setStartValue(QRect(ui->stackedWidget->width()/3,60,ui->stackedWidget->width(),ui->stackedWidget->height()));
            m_stackAnimation->setEndValue(QRect(0,60,ui->stackedWidget->width(),ui->stackedWidget->height()));
            m_stackAnimation->start();
        }else {
            m_stackAnimation->setStartValue(QRect(-ui->stackedWidget->width()/3,60,ui->stackedWidget->width(),ui->stackedWidget->height()));
            m_stackAnimation->setEndValue(QRect(0,60,ui->stackedWidget->width(),ui->stackedWidget->height()));
            m_stackAnimation->start();
        }
        switch(m_lastOrder) {
            case 0:
                m_scrollvalue0 =ui->tableView_courses->verticalScrollBar()->value();
                break;
            case 1:
                m_scrollvalue1 =ui->tableView_courses->verticalScrollBar()->value();
                break;
            case 3:
                m_scrollvalue3 =ui->tableView_courses->verticalScrollBar()->value();
                ui->pushButton_prev->setVisible(true);
                ui->label_page->setVisible(true);\
                ui->lineEdit_page->setVisible(true);
                ui->pushButton_next->setVisible(true);
                ui->pushButton_goto->setVisible(true);
                break;
            default:
                ;
        }

        switch(order) {
            case 0: //全部课程
                ui->tableView_courses->clearSpans();
                ui->tableView_courses->verticalScrollBar()->setValue(m_scrollvalue0);
                ui->lineEdit_page->setText(QString::number(m_page0+1));
                ui->label_page->setText("/"+QString::number(ALLPAGENUMBER)+"页");
                if(m_click0==0) ui->pushButton_flush->setText("刷新");
                else if(m_click0==1) ui->pushButton_flush->setText("正在刷新");
                else ui->pushButton_flush->setText("正在记录");

                m_classModel->setFilter(" Semester='"+m_semester+"' limit "+QString::number(m_page0*LIMIT)+","+QString::number(m_page0*LIMIT+LIMIT));
                mergeCells();
                ui->stackedWidget->setCurrentIndex(0);

                m_lastOrder=0;
                break;
            case 1: //校公选课程
                ui->tableView_courses->clearSpans();
                ui->tableView_courses->verticalScrollBar()->setValue(m_scrollvalue1);
                ui->lineEdit_page->setText(QString::number(m_page1+1));
                ui->label_page->setText("/"+QString::number(PUBLICPAGENUMBER)+"页");
                if(m_click1==0) ui->pushButton_flush->setText("刷新");
                else ui->pushButton_flush->setText("正在刷新");

                m_classModel->setFilter(" Semester='"+m_semester+"' and IsSchoolPublicCourse=1 limit "+QString::number(m_page1*LIMIT)+","+QString::number(m_page1*LIMIT+LIMIT));
                mergeCells();
                ui->stackedWidget->setCurrentIndex(0);

                m_lastOrder=1;
                break;
            case 3: //课程收藏页
                ui->tableView_courses->clearSpans();
                ui->tableView_courses->verticalScrollBar()->setValue(m_scrollvalue3);
                ui->pushButton_prev->setVisible(false);
                ui->label_page->setVisible(false);\
                ui->lineEdit_page->setVisible(false);
                ui->pushButton_next->setVisible(false);
                ui->pushButton_goto->setVisible(false);

                m_classModel->setFilter(" Semester='"+m_semester+"' and (IfStar=1 or IfSend=1)");
                mergeCells();
                ui->stackedWidget->setCurrentIndex(0);

                m_lastOrder=3;
                break;
            case 4: //教材自定义
                ui->stackedWidget->setCurrentIndex(2);

                m_lastOrder=4;
                break;
            case 5: //大物实验选课
                ui->stackedWidget->setCurrentIndex(3);

                m_lastOrder=5;
                break;
            default:
                    ;
        }
    }
    if( order==2 ) {//自由选择,只要点击自由选择按钮以后一定需要保证显示的是自由选择页面
        ui->stackedWidget->setCurrentIndex(1);
        m_lastOrder=2;
    }
}

void StackedSelectClassWidget::on_pushButton_prev_clicked()
{
   //change this
   int currentPage =ui->lineEdit_page->text().toInt();
   QString pageNumberText =ui->label_page->text();
   int pageNumber =pageNumberText.mid(1,pageNumberText.length()-2).toInt();
   if(currentPage<1) currentPage =1;
   else if(currentPage>pageNumber) currentPage =pageNumber;
   else ;
   if(currentPage!=1) {
       ui->lineEdit_page->setText(QString::number(--currentPage));
       switch(m_lastOrder) {
           case 0://全部课程
               m_page0 =currentPage-1;
               m_classModel->setFilter(" Semester='"+m_semester+"' limit "+QString::number(m_page0*LIMIT)+","+QString::number(LIMIT));
               m_scrollvalue0 =0;
               ui->tableView_courses->verticalScrollBar()->setValue(m_scrollvalue0);
               break;
           case 1://校公选课程
               m_page1 =currentPage-1;
               m_classModel->setFilter(" Semester='"+m_semester+"' and IsSchoolPublicCourse=1 limit "+QString::number(m_page1*LIMIT)+","+QString::number(LIMIT));
               m_scrollvalue1 =0;
               ui->tableView_courses->verticalScrollBar()->setValue(m_scrollvalue1);
               break;
           case 2:{//自由选择
               m_page2=currentPage-1;
               QString filter =m_classModel->filter();
               m_classModel->setFilter(filter.left(filter.indexOf("limit"))+" limit "+QString::number(m_page2*LIMIT)+","+QString::number(LIMIT));
               break;
           }default:
               ;
       }

   }
}

void StackedSelectClassWidget::on_pushButton_next_clicked()
{
    //change this
    int currentPage =ui->lineEdit_page->text().toInt();
    QString pageNumberText =ui->label_page->text();
    int pageNumber =pageNumberText.mid(1,pageNumberText.length()-2).toInt();
    if(currentPage<1) currentPage =1;
    else if(currentPage>pageNumber) currentPage =pageNumber;
    else ;
    if(currentPage!=pageNumber) {
        ui->lineEdit_page->setText(QString::number(++currentPage));
        switch(m_lastOrder) {
            case 0://全部课程
                m_page0 =currentPage-1;
                m_classModel->setFilter(" Semester='"+m_semester+"' limit "+QString::number(m_page0*LIMIT)+","+QString::number(LIMIT));
                m_scrollvalue0 =0;
                ui->tableView_courses->verticalScrollBar()->setValue(m_scrollvalue0);
                break;
            case 1://校公选课程
                m_page1 =currentPage-1;
                m_classModel->setFilter(" Semester='"+m_semester+"' and IsSchoolPublicCourse=1 limit "+QString::number(m_page1*LIMIT)+","+QString::number(LIMIT));
                m_scrollvalue1 =0;
                ui->tableView_courses->verticalScrollBar()->setValue(m_scrollvalue1);
                break;
            case 2:{//自由选择
                m_page2=currentPage-1;
                QString filter =m_classModel->filter();
                m_classModel->setFilter(filter.left(filter.indexOf("limit"))+" limit "+QString::number(m_page2*LIMIT)+","+QString::number(LIMIT));
                break;
            }default:
                ;
        }

    }
}

void StackedSelectClassWidget::on_pushButton_goto_clicked()
{
    int currentPage =ui->lineEdit_page->text().toInt();
    QString pageNumberText =ui->label_page->text();
    int pageNumber =pageNumberText.mid(1,pageNumberText.length()-2).toInt();
    if(currentPage<1) currentPage =1;
    else if(currentPage>pageNumber) currentPage =pageNumber;
    else ;
    ui->lineEdit_page->setText(QString::number(currentPage));
    switch(m_lastOrder) {
        case 0://全部课程
            m_page0 =currentPage-1;
            m_classModel->setFilter(" Semester='"+m_semester+"' limit "+QString::number(m_page0*LIMIT)+","+QString::number(LIMIT));
            m_scrollvalue0 =0;
            ui->tableView_courses->verticalScrollBar()->setValue(m_scrollvalue0);
            break;
        case 1://校公选课程
            m_page1 =currentPage-1;
            m_classModel->setFilter(" Semester='"+m_semester+"' and IsSchoolPublicCourse=1 limit "+QString::number(m_page1*LIMIT)+","+QString::number(LIMIT));
            m_scrollvalue1 =0;
            ui->tableView_courses->verticalScrollBar()->setValue(m_scrollvalue1);
            break;
        case 2:{//自由选择
            m_page2=currentPage-1;
            QString filter =m_classModel->filter();
            m_classModel->setFilter(filter.left(filter.indexOf("limit"))+" limit "+QString::number(m_page2*LIMIT)+","+QString::number(LIMIT));
            break;
        }default:
            ;
    }
}

void StackedSelectClassWidget::on_pushButton_profession_clicked()
{
    m_professionChooseForm->show();
}

void StackedSelectClassWidget::handleChoice(QString text)
{
    ui->pushButton_profession->setText(text);

}

void StackedSelectClassWidget::on_pushButton_find_clicked()
{
    QString courseName =ui->lineEdit_CourseName->text();
    QString classId =ui->lineEdit_ClassId->text();
    QString courseId =ui->lineEdit_CourseId->text();
    QString teacher =ui->lineEdit_teacherName->text();
    QString professionofCourse =ui->pushButton_profession->text();
    QString campus =ui->lineEdit_Campus->text();
    QString filter=" Semester='"+m_semester+"' ";
    if(courseName!="") filter += " and CourseName like \'%"+courseName+"%\'";
    if(courseId!="") filter += "and CourseId=\'"+courseId+"\'";
    if(classId!="") filter += " and ClassId=\'"+classId+"\'";
    if(teacher!="") filter += " and Teacher like \'%"+teacher+"%\'";
    if(professionofCourse!="") filter += " and ProfessionofCourse=\'"+professionofCourse+"\'";
    if(campus!="") filter += " and Campus=\'"+campus+"\'";
    m_classModel->setFilter(filter);
    ui->stackedWidget->setCurrentIndex(0);

    QSqlQuery *sqlQuery =new QSqlQuery(QSqlDatabase::database());
    SQLErrorDetect(sqlQuery, "select count(*) from Courses where "+filter, "设置自由选择页数失败 in StackedSelectClassWidget::on_pushButton_find_clicked()");
    sqlQuery->next();
    int recordNumber =sqlQuery->value(0).toInt();
    int pageNumber=(recordNumber==recordNumber/LIMIT*LIMIT&&recordNumber!=0)?recordNumber/LIMIT:recordNumber/LIMIT+1;
    delete sqlQuery;
    ui->label_page->setText("/"+QString::number(pageNumber)+"页");
    m_page2=1;
    ui->lineEdit_page->setText("1");
}

void StackedSelectClassWidget::on_pushButton_materials_send_clicked()
{
    emit sendMaterialInfo();

}



void StackedSelectClassWidget::handleAddCourse(int wantBegin, int wantDay)
{

    QSqlQuery *sqlQuery =new QSqlQuery(QSqlDatabase::database()), *courseSqlQuery=new QSqlQuery(QSqlDatabase::database()), *classScheduleQuery=new QSqlQuery(QSqlDatabase::database());
    SQLErrorDetect(classScheduleQuery, "select TeachingWeek,TeachingDay,TeachingSession from ClassSchedule where Semester='"+m_semester+"' and Username='"+m_username+"';", "获取课程表信息 in StackedSelectClassWidget::handleAddCourse(int wantBegin, int wantDay)");
    QString  newfilter=" Semester='"+m_semester+"' and ( 0 ";
    int wantEnd =0;
    if( wantBegin<=4 )
        wantEnd =4;//上午范围
    else if( wantBegin<=9 )
        wantEnd =9;//下午范围
    else
        wantEnd =12;//晚上范围
    for(int i1=wantBegin;i1<=wantEnd;++i1 ) {
        SQLErrorDetect(sqlQuery, "select * from Courses where TeachingDay='"+QString::number(wantDay)+"' and TeachingSession='"+QString::number(wantBegin)+"~"+QString::number(i1)+"' and Semester='"+m_semester+"';", "选择存在想要时间段的课程 in StackedSelectClassWidget::handleAddCourse(int wantBegin, int wantDay)");
        while ( sqlQuery->next() ) {//存在指定时间段的某一课程
            int notfind =0;
            SQLErrorDetect(courseSqlQuery, "select TeachingWeek,TeachingDay,TeachingSession from Courses where CourseId='"+sqlQuery->value("CourseId").toString()+"' and ClassId='"+sqlQuery->value("ClassId").toString()+"' and Semester='"+m_semester+"';", "获取课程信息失败 in StackedSelectClassWidget::handleAddCourse(int wantBegin, int wantDay)");
            while( courseSqlQuery->next() && notfind==0 ) {//该课程的所有时间段
                while( classScheduleQuery->next() && notfind==0 ) { //将该课程每一个时间段与课程表中的所有时间段进行比较
                    if(judgeConflict(courseSqlQuery->value(0).toString(),classScheduleQuery->value(0).toString()) &&
                       courseSqlQuery->value(1).toString()==classScheduleQuery->value(1).toString() &&
                       judgeConflict(courseSqlQuery->value(2).toString(),classScheduleQuery->value(2).toString()) )
                        notfind =1;
                }
                classScheduleQuery->first();
                classScheduleQuery->previous();
            }
            if( notfind==0 )
                   newfilter += " or CourseId='"+sqlQuery->value("CourseId").toString() + "' and ClassId='"+ sqlQuery->value("ClassId").toString()+"' ";
        }
    }
    newfilter +=");";

    ui->tableView_courses->clearSpans();
    m_classModel->setFilter(newfilter);
    mergeCells();
    ui->lineEdit_page->setText("1");
    ui->label_page->setText("/1页");
    m_lastOrder =-1;
    delete sqlQuery;
    delete courseSqlQuery;
    delete classScheduleQuery;
}



//对周次、上课节数进行判断，返回true表示存在矛盾
/*
* 实际上就是对周次写法中的“-”、“,” 以及 上课节数写法中的“~” 与符合两边的数字 进行范围重合情况的解析
* 如果要整体逻辑简单点，可以将范围表示转换为QList<int>，然后判断其中是否有重复的数字
*/
bool StackedSelectClassWidget::judgeConflict(QString str0, QString str1)
{
    //删除中文的干扰，只保留支持的三种字符与数字
    //经过发现，一般中文只存在在后面加上“周”或者“周上”这两种可能。周次描述可能是“单周”、“双周”
    if(str0.right(1)=="周") str0 =str0.left(str0.length()-1);
    else if(str0.right(1)=="上") str0 =str0.left(str0.length()-2);
    if(str1.right(1)=="周") str1 =str1.left(str1.length()-1);
    else if(str1.right(1)=="上") str1 =str1.left(str1.length()-2);

    if(str0=="单") str0="1,3,5,7,9,11,13,15,17";
    else if(str0=="双") str0="2,4,6,8,10,12,14,16,18";
    if(str1=="单") str1="1,3,5,7,9,11,13,15,17";
    else if(str1=="双") str1="2,4,6,8,10,12,14,16,18";


    QStringList list0 =str0.split(",",QString::SkipEmptyParts);
    QStringList list1 =str1.split(",",QString::SkipEmptyParts);
    int pos0 =0, pos1 =0;
    while( pos0!=list0.size() && pos1!=list1.size() ) {
        int num00,num01,num10,num11;
        QStringList tmpList;
        if( str0.contains("~") )
            tmpList=list0[pos0].split("~");
        else if( str0.contains("-") )
            tmpList=list0[pos0].split("-");
        else
            tmpList << list0[pos0];
        num00 =tmpList[0].toInt();
        num01 =tmpList[tmpList.size()-1].toInt();//num00-num01 范围
        if( str1.contains("~") )
            tmpList=list1[pos1].split("~");
        else if( str1.contains("-") )
            tmpList=list1[pos1].split("-");
        else
            tmpList << list1[pos1];
        num10 =tmpList[0].toInt();
        num11 =tmpList[tmpList.size()-1].toInt();//num10-num11 范围

        if( num01<num10 ) ++pos0;
        else if( num00>num11 ) ++pos1;
        else return true;
    }
    return false;
}

void StackedSelectClassWidget::on_pushButton_addCourse_clicked()
{
    QSqlQuery *sqlQuery =new QSqlQuery(QSqlDatabase::database()), *classScheduleQuery=new QSqlQuery(QSqlDatabase::database()),*courseSqlQuery=new QSqlQuery(QSqlDatabase::database());
    int needQMessageBox=0;QString message="您勾选的下列课程与当前课程表存在时间冲突，未能加入课程表：\n";
    SQLErrorDetect(classScheduleQuery, "select TeachingWeek,TeachingDay,TeachingSession from ClassSchedule where Semester='"+m_semester+"' and Username='"+m_username+"';", "获取课程表信息 in StackedSelectClassWidget::on_pushButton_addCourse_clicked()");
    SQLErrorDetect(sqlQuery, "select * from Courses where IfSend=1 and Semester='"+m_semester+"';", "获取需要临时加入课程表的课程信息 in StackedSelectClassWidget::on_pushButton_addCourse_clicked()");
    while( sqlQuery->next() ){
        int notfind =0;
        SQLErrorDetect(courseSqlQuery, "select TeachingWeek,TeachingDay,TeachingSession from Courses where CourseId='"+sqlQuery->value("CourseId").toString()+"' and ClassId='"+sqlQuery->value("ClassId").toString()+"' and Semester='"+m_semester+"';", "获取课程信息失败 in StackedSelectClassWidget::handleAddCourse(int wantBegin, int wantDay)");
        while( courseSqlQuery->next() && notfind==0 ) {//该课程的所有时间段
            while( classScheduleQuery->next() && notfind==0 ) { //将该课程每一个时间段与课程表中的所有时间段进行比较
                if(judgeConflict(courseSqlQuery->value(0).toString(),classScheduleQuery->value(0).toString()) &&
                   courseSqlQuery->value(1).toString()==classScheduleQuery->value(1).toString() &&
                   judgeConflict(courseSqlQuery->value(2).toString(),classScheduleQuery->value(2).toString()) )
                    notfind =1;
            }
            classScheduleQuery->first();
            classScheduleQuery->previous();
        }
        if( notfind==0 ){
            SQLErrorDetect(courseSqlQuery, "insert into ClassSchedule values('"+m_semester+"'," //Semester
                           "'"+m_username+"',"                                //Username
                           "0,"                                               //State 0表示临时添加进课表
                           "'"+sqlQuery->value("CourseId").toString()+"',"    //CourseId
                           "'"+sqlQuery->value("CourseName").toString()+"',"  //CourseName
                           "'',"                                           //CourseAttribute
                           "'"+sqlQuery->value("ClassId").toString()+"',"     //ClassId
                           "'"+sqlQuery->value("Credir").toString()+"',"      //Credir
                           "'"+sqlQuery->value("TypeofExam").toString()+"',"  //TypeofExam
                           "'"+sqlQuery->value("Teacher").toString()+"',"     //Teacher
                           "'',"                                           //WayofAttending
                           "'',"                                           //State
                           "'"+sqlQuery->value("TeachingWeek").toString()+"',"//TeachingWeek
                           "'"+sqlQuery->value("TeachingDay").toString()+"'," //TeachingDay
                           "'"+sqlQuery->value("TeachingSession").toString()+"',"//TeachingSession
                           "'"+sqlQuery->value("Campus").toString()+"',"      //Campus
                           "'"+sqlQuery->value("TeachingBuilding").toString()+"',"//TeachingBuilding
                           "'"+sqlQuery->value("TeachingRoom").toString()+"');", //TeachingRoom
                           "插入课程表失败 in StackedSelectClassWidget::on_pushButton_addCourse_clicked()");
            emit haveAddedCourse(sqlQuery->value("TeachingSession").toString().split("~")[0].toInt()-1, sqlQuery->value("TeachingDay").toInt()-1,sqlQuery->value("CourseName").toString());
        }else {
            QString courseName =sqlQuery->value("CourseName").toString();
            needQMessageBox =1;
            if(!message.contains(courseName)) message += courseName+"\n";
        }

    }
    if( needQMessageBox==1 ) QMessageBox::warning(nullptr, "", message);
    delete sqlQuery;
    delete courseSqlQuery;
    delete classScheduleQuery;
}


void StackedSelectClassWidget::handleSchoolPublicCoursesError(QString message) {
    QMessageBox::information(nullptr, "告知", message);
    ui->pushButton_flush->setText("刷新");
    m_click1=0;
}

void StackedSelectClassWidget::on_pushButton_flush_clicked()
{
    if( m_lastOrder==0 )  { emit getAllCourses(); m_click0=1;}
    else if(m_lastOrder==1) { emit getSchoolPublicCourses(); m_click1=1;}
    else ;
    ui->pushButton_flush->setText("正在刷新");


}


void StackedSelectClassWidget::flushSchoolPublicCourses() {
    m_classModel->select();
    QSqlQuery *sqlQuery =new QSqlQuery(QSqlDatabase::database());
    SQLErrorDetect(sqlQuery, "select count(*) from Courses where IsSchoolPublicCourse=1 and Semester='"+m_semester+"';", "获取校公选开课数量失败 in StackedSelectClassWidget::setTable()");
    sqlQuery->next();int recordNumber =sqlQuery->value(0).toInt();
    PUBLICPAGENUMBER=(PUBLICPAGENUMBER==recordNumber/LIMIT*LIMIT&&recordNumber!=0)?recordNumber/LIMIT:recordNumber/LIMIT+1;
    delete sqlQuery;
    ui->label_page->setText("/"+QString::number(PUBLICPAGENUMBER)+"页");
    ui->pushButton_flush->setText("刷新完成");
    m_click1 =0;
}

void StackedSelectClassWidget::flushMaterialInfo() {
    m_materialModel->select();
    moveMaterialSendButton();
}

void StackedSelectClassWidget::flushAllCourses() {
    m_classModel->select();
    QSqlQuery *sqlQuery =new QSqlQuery(QSqlDatabase::database());
    SQLErrorDetect(sqlQuery, "select count(*) from Courses where Semester='"+m_semester+"';", "获取所有开课数量失败 in StackedSelectClassWidget::setTable()");
    sqlQuery->next();int recordNumber =sqlQuery->value(0).toInt();
    ALLPAGENUMBER=(recordNumber==recordNumber/LIMIT*LIMIT&&recordNumber!=0)?recordNumber/LIMIT:recordNumber/LIMIT+1;
    delete sqlQuery;
    ui->label_page->setText("/"+QString::number(ALLPAGENUMBER)+"页");
    ui->pushButton_flush->setText("刷新完成");
    m_click0 =0;
}

void StackedSelectClassWidget::flushingAllCourses() {
    ui->pushButton_flush->setText("正在记录");
    m_click0=2;
}
void StackedSelectClassWidget::on_pushButton_flush_material_clicked()
{
    emit getMaterialInfo();
}
