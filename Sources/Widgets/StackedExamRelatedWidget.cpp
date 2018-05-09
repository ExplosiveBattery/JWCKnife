#include "StackedExamRelatedWidget.h"


StackedExamRelatedWidget::StackedExamRelatedWidget(QWidget *parent) :
    QWidget(parent),ui(new Ui::StackedExamRelatedWidget),
    m_stackAnimation(new QPropertyAnimation(this,"geometry")),m_lastOrder(0),m_flush(0),
    m_ExamModel(new QSqlTableModel(this)),m_tableDelegate(new AutoTurnLineDelegate(this)),m_timer(new QTimer())

{
    m_sqlQuery =new QSqlQuery(m_ExamModel->database());
    ui->setupUi(this);
    ui->widget_title->addButtons(QStringList{"考试安排" ,"体育理论考试"});
    connect(ui->widget_title, SIGNAL(stackButtonClick(int)), this, SLOT(handleStackButtonClick(int)) );
    connect( m_timer, SIGNAL(timeout()), this, SLOT(setUnaccessible()) );
    m_stackAnimation->setTargetObject(ui->stackedWidget);
    m_stackAnimation->setDuration(200);

    //page0
    m_ExamModel->setTable("Exams");
    m_ExamModel->setHeaderData(ATTRIBUTE_COLUMN, Qt::Horizontal,  "考试名");
    m_ExamModel->setHeaderData(CAMPUS_COLUMN, Qt::Horizontal,  "校区");
    m_ExamModel->setHeaderData(BUILDING_COLUMN, Qt::Horizontal,  "教学楼");
    m_ExamModel->setHeaderData(ROOM_COLUMN, Qt::Horizontal,  "教室");
    m_ExamModel->setHeaderData(COURSENAME_COLUMN, Qt::Horizontal,  "课程名");
    m_ExamModel->setHeaderData(WEEK_COLUMN, Qt::Horizontal,  "周次");
    m_ExamModel->setHeaderData(DAY_COLUMN, Qt::Horizontal,  "星期");
    m_ExamModel->setHeaderData(DATE_COLUMN, Qt::Horizontal,  "日期");
    m_ExamModel->setHeaderData(TIME_COLUMN, Qt::Horizontal,  "时间");
    m_ExamModel->setHeaderData(POSITIONID_COLUMN, Qt::Horizontal,  "座位号");
    ui->tableView->setModel(m_ExamModel);
    ui->tableView->hideColumn(SEMESTER_COLUMN);
    ui->tableView->hideColumn(USERNAME_COLUMN);
    for(int i1=ATTRIBUTE_COLUMN;i1 <= POSITIONID_COLUMN;++i1)
        ui->tableView->setItemDelegateForColumn(i1, m_tableDelegate);

    //page1
    connect(ui->lineEdit,SIGNAL(textChanged(const QString &)), this, SLOT(displayResult(const QString &)));

}

StackedExamRelatedWidget::~StackedExamRelatedWidget()
{
    delete ui;
    delete m_sqlQuery;
}

void StackedExamRelatedWidget::init()
{
    m_ExamModel->setFilter(" Semester='"+m_semester+"' and Username='"+m_username+"' ");
    m_ExamModel->select();
}

void StackedExamRelatedWidget::setSemester(QString semester)
{
    m_semester =semester;
}


void StackedExamRelatedWidget::setUsername(QString username)
{
    m_username =username;
}

void StackedExamRelatedWidget::handleStackButtonClick(int order) {
    if( m_lastOrder!=order ) {
        if( order==0 ) {
            m_stackAnimation->setStartValue(QRect(ui->stackedWidget->width()/3,60,ui->stackedWidget->width(),ui->stackedWidget->height()));
            m_stackAnimation->setEndValue(QRect(0,60,ui->stackedWidget->width(),ui->stackedWidget->height()));
            m_stackAnimation->start();
        }else {
            m_stackAnimation->setStartValue(QRect(-ui->stackedWidget->width()/3,60,ui->stackedWidget->width(),ui->stackedWidget->height()));
            m_stackAnimation->setEndValue(QRect(0,60,ui->stackedWidget->width(),ui->stackedWidget->height()));
            m_stackAnimation->start();
        }
        switch (order) {
            case 0:
                ui->stackedWidget->setCurrentIndex(0);
                break;
            case 1:
                ui->stackedWidget->setCurrentIndex(1);
                break;
            default:
                ;
        }
        m_lastOrder =order;
    }
}


void StackedExamRelatedWidget::displayResult(const QString &str) {
    if(str=="") {
        ui->textBrowser->setText("");
        ui->label->setText("共0条结果");
        return;
    }
    SQLErrorDetect(m_sqlQuery, "select * from PE_exam where question like '%"+str+"%';", "体育题库搜索出错 in StackedExamRelatedWidget::displayResult(const QString &)");
    QString result; int count =0;
    while ( m_sqlQuery->next() ) {
        ++count;
        result +=QString::number(count)+". "+m_sqlQuery->value("question").toString()+"\n\t";
        if(m_sqlQuery->value("A").toInt()==1)
            result +="A.";
        else  if(m_sqlQuery->value("B").toInt()==1)
            result +="B.";
        else  if(m_sqlQuery->value("C").toInt()==1)
            result +="C.";
        else  if(m_sqlQuery->value("D").toInt()==1)
            result +="D.";
        else
            result +="";
        result +=m_sqlQuery->value("answer").toString()+"\n";
    }
    if( count < 10 ) {
        ui->textBrowser->setText(result);
        ui->label->setText("共"+QString::number(count)+"条结果");
    }else {
        ui->textBrowser->setText("匹配的内容过多，请输入更加精准的题干");
        ui->label->setText("共"+QString::number(count)+"条结果");
    }


}

void StackedExamRelatedWidget::on_pushButton_flush_clicked()
{
    if(m_flush==0) {
        m_timer->start(5000);
        m_flush =1;
        ui->label_flush->setText("正在刷新");
        emit getExamInfo(true);
    }
}


void StackedExamRelatedWidget::flushExamInfo()
{
    m_ExamModel->select();
    if( m_flush==1 ) {
        ui->label_flush->setText("刷新完成");
        m_timer->stop();
        m_flush =0;
    }
}


void StackedExamRelatedWidget::setUnaccessible()
{
    ui->label_flush->setText("刷新超时");
    m_flush =0;
}
