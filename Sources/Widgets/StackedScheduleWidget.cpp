#include "StackedScheduleWidget.h"
#include "ui_StackedScheduleWidget.h"

#include <QDebug>

StackedScheduleWidget::StackedScheduleWidget(QWidget *parent) :
    QWidget(parent),ui(new Ui::StackedScheduleWidget),
    m_pQuery(new QSqlQuery(QSqlDatabase::database())),m_flush(0),m_timer(new QTimer())
{
    //随机数支持
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    //字体引入
    int fontId =QFontDatabase::addApplicationFont(":/Resources/titlewidget.ttf");
    QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
    m_font.setFamily(fontFamilies.at(0));
    m_font.setPointSize(12);
    //界面设置
    ui->setupUi(this);
    ui->tableWidget_schedule_courses->setRowCount(12);
    ui->tableWidget_schedule_courses->setColumnCount(7);
    QStringList horizontalHeader_schedule{"周一","周二","周三","周四","周五","周六", "周日"};
    ui->tableWidget_schedule_courses->setHorizontalHeaderLabels(horizontalHeader_schedule);
    ui->tableWidget_schedule_courses->horizontalHeader()->setDefaultSectionSize(80);
    ui->tableWidget_schedule_courses->verticalHeader()->setDefaultSectionSize(49);
    ui->tableWidget_schedule_courses->setFont(m_font);
    ui->tableWidget_schedule_courses->setContextMenuPolicy (Qt::CustomContextMenu);
    ui->tableWidget_schedule_courses->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(ui->tableWidget_schedule_courses,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(showMenu(QPoint)));
    connect(m_timer, SIGNAL(timeout()), this, SLOT(setUnaccessible()) );
}

StackedScheduleWidget::~StackedScheduleWidget()
{
    delete ui;
}



void StackedScheduleWidget::setSemester(QString semester)
{
    m_semester =semester;
}

void StackedScheduleWidget::setUsername(QString username)
{
    m_username =username;
}

void StackedScheduleWidget::fillTablewidget() {
    fillScheduleTablewidget(1);
    fillScheduleTablewidget(0);
}

void StackedScheduleWidget::fillScheduleTablewidget(int num)
{
    SQLErrorDetect(m_pQuery, "select * from ClassSchedule where TeachingWeek is not null and TeachingDay is not null and TeachingSession is not null and State="+QString::number(num)+" and Semester='"+m_semester+"' and Username='"+m_username+"';", "获取课程表信息失败 in StackedScheduleWidget::fillScheduleTablewidget(QWidget *parent)");
    QSqlQuery *sqlQuery =new QSqlQuery(QSqlDatabase::database());
    while( m_pQuery->next() ) {
        int teachingDay =m_pQuery->value("TeachingDay").toInt(), teachingBegin =0, teachingLength =0;
        QString teachingSession =m_pQuery->value("TeachingSession").toString(),display="";
        if( teachingSession.length()==3 ) {
            teachingBegin =teachingSession.left(1).toInt();
            teachingLength =teachingSession.right(1).toInt()-teachingBegin+1;
        }else if( teachingSession.length()==5 ) {
            teachingBegin =teachingSession.left(2).toInt();
            teachingLength =teachingSession.right(2).toInt()-teachingBegin+1;
        }
        SQLErrorDetect(sqlQuery, "select CourseName from ClassSchedule where TeachingDay="+QString::number(teachingDay)+" and TeachingSession='"+teachingSession+"' and Semester='"+m_semester+"' and Username='"+m_username+"';", "查询同一时间内的课程名字 in StackedScheduleWidget::StackedScheduleWidget(QWidget *parent)");
        while( sqlQuery->next() )
           display +=sqlQuery->value(0).toString()+"\n";

        ui->tableWidget_schedule_courses->setItem(teachingBegin-1, teachingDay-1,  new QTableWidgetItem(display));
        ui->tableWidget_schedule_courses->setSpan(teachingBegin-1, teachingDay-1, teachingLength, 1);
        if(num!=0)
            ui->tableWidget_schedule_courses->item(teachingBegin-1, teachingDay-1)->setBackground( m_vColor[qrand()%m_vColor.size()] );
        else
            ui->tableWidget_schedule_courses->item(teachingBegin-1, teachingDay-1)->setBackground(m_specialColor);
    }
    delete sqlQuery;
}



void StackedScheduleWidget::showMenu(const QPoint pos)
{
    //设置菜单选项
    QMenu *menu = new QMenu(ui->tableWidget_schedule_courses);
    m_menuIndex = ui->tableWidget_schedule_courses->indexAt(QPoint(pos.x (),pos.y ()));
    QAction *action = new QAction("添加课程",ui->tableWidget_schedule_courses);
    menu->addAction(action);
    connect (action,SIGNAL(triggered()),this,SLOT(handleAction()));
    connect(action, SIGNAL(hovered()),this,SLOT(test()));
    if(m_menuIndex.data().toString()!="") {
        QStringList stringList=m_menuIndex.data().toString().split("\n",QString::SkipEmptyParts);
        for(auto str : stringList) {
            QAction *action = new QAction("删除"+str,ui->tableWidget_schedule_courses);
            menu->addAction(action);
            connect (action,SIGNAL(triggered()),this,SLOT(handleAction()));
        }
    }


    menu->move(cursor().pos());
    menu->show();
}


void StackedScheduleWidget::handleAction()
{

    if( static_cast<QAction*>(sender())->text()=="添加课程" ) {
        emit addCourse(m_menuIndex.row()+1, m_menuIndex.column()+1);
    }else {
        QString str =static_cast<QAction*>(sender())->text();
        str =str.right(str.length()-2);
        SQLErrorDetect(m_pQuery,"select State from ClassSchedule where CourseName='"+str+"' and Semester='"+m_semester+"' and Username='"+m_username+"';", "查询指定课程State in StackedScheduleWidget::handleAction0()");
        m_pQuery->next();
        if( m_pQuery->value(0).toInt()==0 ) SQLErrorDetect(m_pQuery,"delete from ClassSchedule where CourseName='"+str+"' and Semester='"+m_semester+"' and Username='"+m_username+"';", "删除课表中临时添加的课程失败 in StackedScheduleWidget::handleAction0()");
        else SQLErrorDetect(m_pQuery,"update ClassSchedule set State=2 where CourseName='"+str+"' and Semester='"+m_semester+"' and Username='"+m_username+"';", "临时删除课表中选中课程失败 in StackedScheduleWidget::handleAction0()");
        ui->tableWidget_schedule_courses->clearContents();
        ui->tableWidget_schedule_courses->clearSpans();
        fillTablewidget();
    }
}

void StackedScheduleWidget::handleHaveAddedCourse(int row, int column, QString courseName) {
    fillScheduleTablewidget(0);//这个函数会对所有的IfHas=0的课程记录进行setItem，到了后期肯定会有不少重复
}

void StackedScheduleWidget::on_pushButton_flush_clicked()
{
    if( m_flush==0 ) {
        m_timer->start(5000);
        m_flush =1;
        ui->label_flush->setText("正在刷新");
        emit getClassSchedule(true);
    }
}


void StackedScheduleWidget::flushClassSchedule(void)
{
    ui->tableWidget_schedule_courses->clearContents();
    ui->tableWidget_schedule_courses->clearSpans();
    fillScheduleTablewidget(1);
    fillScheduleTablewidget(0);
    if(m_flush==1) {
        ui->label_flush->setText("刷新完成");
        m_flush =0;
        m_timer->stop();
    }
}


void StackedScheduleWidget::setUnaccessible()
{
    ui->label_flush->setText("刷新超时");
    m_flush =0;
}


void StackedScheduleWidget::handleNeedEvaluateError() {
    ui->label_flush->setText("请先评教");
}

void StackedScheduleWidget::on_pushButton_back_clicked()
{
    SQLErrorDetect(m_pQuery, "delete from ClassSchedule where State=0;", "删除临时添加的课程失败 in StackedScheduleWidget::on_pushButton_back_clicked()");
    SQLErrorDetect(m_pQuery, "update ClassSchedule set State=1 where State=2;", "恢复临时删除的课程失败 in StackedScheduleWidget::on_pushButton_back_clicked()");
    ui->tableWidget_schedule_courses->clearContents();
    ui->tableWidget_schedule_courses->clearSpans();
    fillTablewidget();
}

void StackedScheduleWidget::on_pushButton_send_clicked()
{
    emit sendClassInfo();

}
