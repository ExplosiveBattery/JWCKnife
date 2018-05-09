#include "StackedGradesWidget.h"
#include "ui_StackedGradesWidget.h"
#include <QDebug>
#include <QMessageBox>

StackedGradesWidget::StackedGradesWidget(QWidget *parent) :
    QWidget(parent),ui(new Ui::StackedGradesWidget),m_stackedWidget(new QStackedWidget(this)),
    m_evaluationDelegate(new EvaluationDelegate(this)), m_tableDelegate(new AutoTurnLineDelegate(this)),
    m_lastOrder(0),m_sqlQuery(new QSqlQuery(QSqlDatabase::database())),m_flush(0),m_timer(new QTimer())
{
    //字体引入
    int fontId =QFontDatabase::addApplicationFont(":/Resources/titlewidget.ttf");
    QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
    m_font.setFamily(fontFamilies.at(0));
    m_font.setPointSize(16);
    //基本设置
    ui->setupUi(this);
    ui->buttonGroup->button(-2)->setChecked(true);
    m_stackedWidget->setGeometry(0,5,1000,670);
    m_timer->setSingleShot(true);
    connect(ui->buttonGroup,SIGNAL(buttonClicked(int)),this,SLOT(handleStackButtonClick(int)));
    connect(m_timer, SIGNAL(timeout()), this, SLOT(setUnaccessible()) );
}

StackedGradesWidget::~StackedGradesWidget()
{
    delete ui;
    delete m_sqlQuery;
}

void StackedGradesWidget::setUsername(QString username)
{
    m_username =username;
}


void StackedGradesWidget::handleStackButtonClick(int order) {
    if( m_lastOrder!=order )
        m_stackedWidget->setCurrentIndex(-2-order);
     m_lastOrder =order;
}

void StackedGradesWidget::fillStackedWidget()
{
    //如果给每一个tableWidget增加一个全选按钮，需要每一个tableWidget各自有一个独立的CheckboxHeader
    QStringList header; header << "选择" << "课程号" << "课序号" << "课程名" << "学分" << "属性" << "分数" << "不及格原因";
    //按照学期显示成绩
    QScrollArea *page0ScrollArea =new QScrollArea(m_stackedWidget);
    page0ScrollArea->setGeometry(0,0,1000,670);
    page0ScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    page0ScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    page0ScrollArea->setFrameShape(QFrame::NoFrame);
    QWidget *page0 =new QWidget(m_stackedWidget);
    QVBoxLayout *page0Layout =new QVBoxLayout;
    int page0Height=0;QStringList m_page0Semster;
    SQLErrorDetect(m_sqlQuery, "select Semester from Grades where Username='"+m_username+"' group by Semester;", "从成绩表中读取信息失败 in StackedGradesWidget(QWidget *parent)");
    while( m_sqlQuery->next() )
        m_page0Semster << m_sqlQuery->value(0).toString();
    for( auto title : m_page0Semster ) {
        QLineEdit *lineEdit =new QLineEdit(page0);
        lineEdit->setText(title);lineEdit->setFont(m_font);lineEdit->setFrame(false);lineEdit->setReadOnly(true);
        QTableWidget *tableWidget =new QTableWidget(100,7,page0);
        fillTableWidget(tableWidget, m_sqlQuery, "select IfChecked,CourseId,ClassId,CourseName,Credir,Attribute,Grade,Reason from Grades where Semester='"+title+"' and Username='"+m_username+"';","从成绩表中读取信息填充table失败 in StackedGradesWidget(QWidget *parent)");
        tableWidget->setHorizontalHeaderLabels(header);
        tableWidget->setMinimumSize(800, tableWidget->rowCount()*30+30);
        tableWidget->hideColumn(IFCHECKED_COLUMN);
        tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

        page0Layout->addWidget(lineEdit);
        page0Layout->addWidget(tableWidget);page0Layout->setAlignment(tableWidget, Qt::AlignHCenter);
        page0Layout->addStretch(1);

        page0Height +=lineEdit->height()+tableWidget->height()+60;
    }
    page0->setGeometry(0,0,1000,page0Height);
    page0->setLayout(page0Layout);
    page0ScrollArea->setWidget(page0);
    m_stackedWidget->addWidget(page0ScrollArea);



    //按照课程属性显示成绩
    QScrollArea *page1ScrollArea =new QScrollArea(m_stackedWidget);
    page1ScrollArea->setGeometry(0,0,1000,670);
    page1ScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    page1ScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    page1ScrollArea->setFrameShape(QFrame::NoFrame);
    QWidget *page1 =new QWidget(m_stackedWidget);
    QVBoxLayout *page1Layout =new QVBoxLayout;
    int page1Height=0;QStringList m_page1Semster;
    SQLErrorDetect(m_sqlQuery, "select Attribute from Grades where Username='"+m_username+"' group by Attribute ;", "从成绩表中读取信息失败 in StackedGradesWidget(QWidget *parent)");
    while( m_sqlQuery->next() )
        m_page1Semster << m_sqlQuery->value(0).toString();
    for( auto title : m_page1Semster ) {
        QLineEdit *lineEdit =new QLineEdit(page1);
        lineEdit->setText(title);lineEdit->setFont(m_font);lineEdit->setFrame(false);lineEdit->setReadOnly(true);
        QTableWidget *tableWidget =new QTableWidget(100,7,page1);
        fillTableWidget(tableWidget, m_sqlQuery, "select IfChecked,CourseId,ClassId,CourseName,Credir,Attribute,Grade,Reason from Grades where Attribute='"+title+"' and Username='"+m_username+"';","从成绩表中读取信息填充table失败 in StackedGradesWidget(QWidget *parent)");
        tableWidget->setHorizontalHeaderLabels(header);
        tableWidget->setMinimumSize(800, tableWidget->rowCount()*30+30);

        tableWidget->hideColumn(IFCHECKED_COLUMN);
        tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

        page1Layout->addWidget(lineEdit);
        page1Layout->addWidget(tableWidget);page1Layout->setAlignment(tableWidget, Qt::AlignHCenter);
        page1Layout->addStretch(1);

        page1Height +=lineEdit->height()+tableWidget->height()+60;
    }
    page1->setGeometry(0,0,1000,page1Height);
    page1->setLayout(page1Layout);
    page1ScrollArea->setWidget(page1);
    m_stackedWidget->addWidget(page1ScrollArea);
}


//鼠标点击以后的状态state只会是0或2。未选择0以及半选1点击了以后都会变成2，全选2会变成未选择0。
void StackedGradesWidget::handleHeaderCheckBoxClicked(int state) {
    QString  title =sender()->objectName();

    if( m_page0Semster.contains(title) ) {
        SQLErrorDetect(m_sqlQuery, "update Grades set IfChecked="+QString::number(state)+" where Semester='"+title+"' and Username='"+m_username+"';", "按照全选设置Grades中内容为0失败 in StackedGradesWidget::handleHeaderCheckBox()");
    }else if( m_page1Semster.contains(title) ) {
        SQLErrorDetect(m_sqlQuery, "update Grades set IfChecked="+QString::number(state)+" where Attribute='"+title+"' and Username='"+m_username+"';", "按照全选设置Grades中内容为1失败 in StackedGradesWidget::handleHeaderCheckBox()");
    }
}


void StackedGradesWidget::on_pushButton_clicked()
{
    QMessageBox::information(this, "告知", "绩点计算公式即将发生改变，本功能或在下一版本中开启");
}

void StackedGradesWidget::on_pushButton_flush_clicked()
{
    if( m_flush==0 ) {
        m_timer->start(5000);
        SQLErrorDetect(m_sqlQuery, "select Attribute from Grades where Grade='' and Username='"+m_username+"';", "获取成绩不全的学期名字");
        while( m_sqlQuery->next() )
            SQLErrorDetect(m_sqlQuery, "delete from Grades where Attribute='"+m_sqlQuery->value(0).toString()+"' and Username='"+m_username+"';", "获取成绩不全的学期名字");
        emit getGrades();
        m_flush =1;
    }
}

void StackedGradesWidget::flushGrades() {
    if( m_flush==1 ) {
        //要想不重启程序进行刷新，代码有点麻烦
        QMessageBox::information(this, "", "刷新已完成，请重新启动程序！");
        m_timer->stop();
        m_flush =0;
    }
}


void StackedGradesWidget::setUnaccessible()
{
    QMessageBox::information(this, "", "刷新超时，可能是网络问题");
    m_flush =0;
    m_timer->stop();
}
