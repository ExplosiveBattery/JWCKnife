#include "MainWindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "StackedPlanCoursesWidget.h"


MainWindow::MainWindow(EducationSystem *pAccount,QWidget *parent) :
    QMainWindow(parent),ui(new Ui::MainWindow),m_pAccount(pAccount)
{
    ui->setupUi(this);
    m_selectedStackButtonX =ui->stackButton0->x();
    this->setWindowFlags(Qt::FramelessWindowHint);
    connect(ui->stackButton0, SIGNAL(clicked()), this, SLOT(handleStackButtonClick()) );
    connect(ui->stackButton1, SIGNAL(clicked()), this, SLOT(handleStackButtonClick()) );
    connect(ui->stackButton2, SIGNAL(clicked()), this, SLOT(handleStackButtonClick()) );
    connect(ui->stackButton3, SIGNAL(clicked()), this, SLOT(handleStackButtonClick()) );
    connect(ui->stackButton4, SIGNAL(clicked()), this, SLOT(handleStackButtonClick()) );
    connect(ui->stackButton5, SIGNAL(clicked()), this, SLOT(handleStackButtonClick()) );

    //add stacked widget
    ui->stackedWidget->setCurrentIndex(0);
    QString semester =m_pAccount->getSemester(), username =m_pAccount->getUsername();
    ui->page0->setSemester(semester);
    ui->page0->setUsername(username);
    ui->page0->fillTablewidget();
    ui->page1->setUsername(username);
    ui->page1->fillStackedWidget();
    ui->page2->addButtons(QVector<int>{3,3},QStringList{"公共课", "专业基础课", "专业课", "实践环节", "跨专业选修课", "中华文化"});
    ui->page2->setUsername(username);
    ui->page3->setSemester(semester);
    ui->page3->setUsername(username);
    ui->page3->init();
    ui->page4->setSemester(semester);
    ui->page4->setUsername(username);
    ui->page4->setTable();
    ui->page4->moveMaterialSendButton();
    ui->page5->setSemester(semester);
    ui->page5->setUsername(username);
    ui->page5->init();
    connect(m_pAccount, SIGNAL(unaccessible()),this, SLOT(handleUnaccessible()));
    connect(ui->page0, SIGNAL(addCourse(int,int)), ui->page4, SLOT(handleAddCourse(int,int)));
    connect(ui->page0, SIGNAL(addCourse(int,int)), this, SLOT(handleAddCourse()));
    connect(ui->page0, SIGNAL(getClassSchedule(bool)), m_pAccount, SLOT(getClassSchedule(bool)));
    connect(m_pAccount, SIGNAL(haveGetClassSchedule()), ui->page0, SLOT(flushClassSchedule()));
    connect(m_pAccount, SIGNAL(selectClassResult(QString)), ui->page0, SLOT(flushClassSchedule()));
    connect(ui->page0, SIGNAL(sendClassInfo()), m_pAccount, SLOT(sendClassInfo()));
    connect(m_pAccount, SIGNAL(needEvaluateError()), ui->page0, SLOT(handleNeedEvaluateError()));
    connect(ui->page1, SIGNAL(getGrades()), m_pAccount, SLOT(getGrades()));
    connect(m_pAccount, SIGNAL(haveGetGrades()), ui->page1, SLOT(flushGrades()));
    connect(ui->page3, SIGNAL(sendEvaluateInfo(QStringList)), m_pAccount, SLOT(sendEvaluateInfo(QStringList)));
    connect(ui->page3, SIGNAL(flushEvaluationInfo(bool)), m_pAccount, SLOT(getEvaluateInfo(bool)));
    connect(m_pAccount, SIGNAL(haveGetEvaluation()), ui->page3, SLOT(flushEvaluationInfo()));
    connect(m_pAccount, SIGNAL(evaluationResult(QString)), this, SLOT(informationWidget(QString)));
    connect(ui->page4, SIGNAL(haveAddedCourse(int,int,QString)), ui->page0, SLOT(handleHaveAddedCourse(int,int,QString)));
    connect(ui->page4, SIGNAL(getAllCourses()), m_pAccount,SLOT(getAllCourses()));
    connect(m_pAccount, SIGNAL(beginRecordAllCourses()), ui->page4, SLOT(flushingAllCourses()));
    connect(m_pAccount, SIGNAL(haveGetAllCourses()), ui->page4, SLOT(flushAllCourses()));
    connect(ui->page4, SIGNAL(getSchoolPublicCourses()), m_pAccount,SLOT(getSchoolPublicCourses()));
    connect(m_pAccount, SIGNAL(haveGetSchoolPublicCourses()), ui->page4, SLOT(flushSchoolPublicCourses()));
    connect(ui->page4, SIGNAL(getMaterialInfo()), m_pAccount,SLOT(getMaterialInfo()));
    connect(ui->page4, SIGNAL(sendMaterialInfo()), m_pAccount, SLOT(sendMaterialInfo()));
    connect(m_pAccount, SIGNAL(haveGetMaterialInfo()), ui->page4, SLOT(flushMaterialInfo()));
    connect(m_pAccount, SIGNAL(getSchoolPublicCoursesError(QString)), ui->page4, SLOT(handleSchoolPublicCoursesError(QString)));
    connect(ui->page5, SIGNAL(getExamInfo(bool)), m_pAccount, SLOT(getExamInfo(bool)));
    connect(m_pAccount, SIGNAL(haveGetExamInfo()), ui->page5, SLOT(flushExamInfo()));
    connect(m_pAccount, SIGNAL(sendClassResult(QString)), this, SLOT(informationWidget(QString)));
    connect(m_pAccount, SIGNAL(selectClassResult(QString)), this, SLOT(informationWidget(QString)));
    connect(m_pAccount, SIGNAL(deleteClassResult(QString)), this, SLOT(informationWidget(QString)));
    connect(m_pAccount, SIGNAL(selectMaterialResult(QString)), this, SLOT(informationWidget(QString)));
    connect(m_pAccount, SIGNAL(timerMessage(QString)), this, SLOT(informationWidget(QString)));
}



MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
         m_drag = true;
         m_windowPosition = event->globalPos() - this->pos();
         event->accept();
     }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (m_drag && event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - m_windowPosition);
        event->accept();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *)
{
    m_drag = false;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPixmap pixTitleTriangle(":/Resources/MainWindow/title_triangle.png");
    QPainter painter(this);
    painter.setBrush(QColor(155,144,194));
    painter.setPen(Qt::NoPen);
    painter.drawRect(ui->title_widget->rect());
    painter.setBrush(QColor(110,117,164));
    painter.drawRect(QRect(m_selectedStackButtonX-20, 0, ui->stackButton0->width()+40, ui->title_widget->height()));
    painter.drawPixmap(m_selectedStackButtonX+15,ui->title_widget->height()-10,pixTitleTriangle);
}

void MainWindow::handleStackButtonClick(void)
{
    int index =sender()->objectName().at(11).digitValue();
    ui->stackedWidget->setCurrentIndex(index);
    m_selectedStackButtonX =((QPushButton*)sender())->x();
    update();
}

void MainWindow::on_pushButton_minimize_clicked()
{
    if(!isMinimized())
        showMinimized();
}

void MainWindow::handleEvaluationSend(QStringList userContent) {
    emit sendEvaluateInfo(userContent);
}


void MainWindow::handleSendClass() {
    emit sendClassInfo();
}


void MainWindow::handleAddCourse() {
    ui->stackButton4->click();
}

void MainWindow::informationWidget(QString message)  {
    QMessageBox::information(nullptr,"告知",message);
}


void MainWindow::handleUnaccessible() {
    QMessageBox::critical(nullptr,"告知", "本科系统没有开放");
}


