#include "StackedEvaluationWidget.h"
#include <QDebug>


StackedEvaluationWidget::StackedEvaluationWidget(QWidget *parent) :
    QWidget(parent),ui(new Ui::StackedEvaluationWidget),m_lastOrder(0),
    m_evaluationModel(new QSqlTableModel(this)),m_evaluationDelegate(new EvaluationDelegate(this)), m_tableDelegate(new AutoTurnLineDelegate(this)),
    m_recordNumber(0),m_sqlQuery(new QSqlQuery(m_evaluationModel->database())),m_flush(0),m_timer(new QTimer())
{
    ui->setupUi(this);
    m_checkboxHeader =new CheckBoxHeader(Qt::Horizontal, ui->tableView_evaluation);
    ui->tableView_evaluation->setHorizontalHeader(m_checkboxHeader);
    connect(m_checkboxHeader, SIGNAL(checkBoxClicked(int)), this, SLOT(handleHeaderCheckBoxClicked(int)) );
    connect(m_evaluationModel, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)), this, SLOT(handleDataChanged()) );
    connect( m_timer, SIGNAL(timeout()), this, SLOT(setUnaccessible()) );
    connect(ui->buttonGroup, SIGNAL(buttonClicked(int)), this, SLOT(handleStackButtonClick(int)));
    connect(ui->buttonGroup_1, SIGNAL(buttonClicked(int)), this, SLOT(handleStackButtonClick(int)));
    connect(ui->buttonGroup_2, SIGNAL(buttonClicked(int)), this, SLOT(handleStackButtonClick(int)));
    connect(ui->buttonGroup_3, SIGNAL(buttonClicked(int)), this, SLOT(handleStackButtonClick(int)));
    connect(ui->buttonGroup_4, SIGNAL(buttonClicked(int)), this, SLOT(handleStackButtonClick(int)));
    connect(ui->buttonGroup_5, SIGNAL(buttonClicked(int)), this, SLOT(handleStackButtonClick(int)));
    connect(ui->buttonGroup_6, SIGNAL(buttonClicked(int)), this, SLOT(handleStackButtonClick(int)));
    connect(ui->buttonGroup_7, SIGNAL(buttonClicked(int)), this, SLOT(handleStackButtonClick(int)));

    ui->stackedWidget->setCurrentIndex(0);
    ui->buttonGroup->button(-2)->setChecked(true);

    ui->radioButton_q1_1->setChecked(true);
    ui->radioButton_q2_1->setChecked(true);
    ui->radioButton_q3_1->setChecked(true);
    ui->radioButton_q4_1->setChecked(true);
    ui->radioButton_q5_1->setChecked(true);
    ui->radioButton_q6_1->setChecked(true);
    ui->radioButton_q7_1->setChecked(true);


    m_evaluationModel->setTable("Evaluation");
    m_evaluationModel->setHeaderData(IFSEND_COLUMN, Qt::Horizontal,"");
    m_evaluationModel->setHeaderData(PERSON_COLUMN, Qt::Horizontal,"被评人");
    m_evaluationModel->setHeaderData(COURSENAME_COLUMN, Qt::Horizontal,"负责课程");

    ui->tableView_evaluation->setModel(m_evaluationModel);
    ui->tableView_evaluation->hideColumn(SEMESTER_COLUMN);
    ui->tableView_evaluation->hideColumn(USERNAME_COLUMN);
    ui->tableView_evaluation->hideColumn(TYPE_COLUMN);
    ui->tableView_evaluation->hideColumn(WJBM_COLUMN);
    ui->tableView_evaluation->hideColumn(BPR_COLUMN);
    ui->tableView_evaluation->hideColumn(PGNR_COLUMN);
    ui->tableView_evaluation->setColumnWidth(IFSEND_COLUMN, 50);
    ui->tableView_evaluation->setColumnWidth(PERSON_COLUMN, 100);
    ui->tableView_evaluation->setColumnWidth(COURSENAME_COLUMN, 200);
    ui->tableView_evaluation->setItemDelegateForColumn(IFSEND_COLUMN, m_evaluationDelegate);
    for(int i1=PERSON_COLUMN; i1<=COURSENAME_COLUMN; ++i1)
        ui->tableView_evaluation->setItemDelegateForColumn(i1, m_tableDelegate);
}


StackedEvaluationWidget::~StackedEvaluationWidget() {
    delete m_sqlQuery;
}

void StackedEvaluationWidget::init() {
    m_evaluationModel->setFilter("Type=1 and Semester='"+m_semester+"' and Username='"+m_username+"' ");
    m_evaluationModel->select();

    SQLErrorDetect(m_sqlQuery, "select count(*) from Evaluation where Semester='"+m_semester+"' and Username='"+m_username+"';", "获取评教信息数量失败 in StackedEvaluationWidget::StackedEvaluationWidget(QWidget *parent)");
    m_sqlQuery->next();
    m_recordNumber =m_sqlQuery->value(0).toInt();
    handleDataChanged();
}


void StackedEvaluationWidget::setSemester(QString semester)
{
    m_semester =semester;
}


void StackedEvaluationWidget::setUsername(QString username)
{
    m_username =username;
}

//鼠标点击以后的状态state只会是0或2。未选择0以及半选1点击了以后都会变成2，全选2会变成未选择0。
void StackedEvaluationWidget::handleHeaderCheckBoxClicked(int state) {
    if( state==0 ) {
        SQLErrorDetect(m_sqlQuery, "update Evaluation set IfSend=0 where Semester='"+m_semester+"' and Username='"+m_username+"' and IfSend=1;", "设置Evaluation中全部内容为0失败 in StackedEvaluationWidget::handleHeaderCheckBox()");
    }else if( state==2 ) {
        SQLErrorDetect(m_sqlQuery, "update Evaluation set IfSend=1 where Semester='"+m_semester+"' and Username='"+m_username+"' and IfSend=0;", "设置Evaluation中全部内容为1失败 in StackedEvaluationWidget::handleHeaderCheckBox()");
    }
    m_evaluationModel->select();
}


void StackedEvaluationWidget::handleDataChanged() {
    SQLErrorDetect(m_sqlQuery, "select count(*) from Evaluation where IfSend=1 and Semester='"+m_semester+"' and Username='"+m_username+"';", "获取评教已选信息数量失败 in StackedEvaluationWidget::handleDataChanged()");
    m_sqlQuery->next();
    int nIfSend =m_sqlQuery->value(0).toInt();
    if( nIfSend==0 ) {
        m_checkboxHeader->setCheckState(0);
    }else if( nIfSend==m_recordNumber ){
        m_checkboxHeader->setCheckState(2);
    }else {
        m_checkboxHeader->setCheckState(1);
    }
}


void StackedEvaluationWidget::on_pushButton_reset_clicked() {
    ui->radioButton_q1_1->setChecked(true);
    ui->radioButton_q2_1->setChecked(true);
    ui->radioButton_q3_1->setChecked(true);
    ui->radioButton_q4_1->setChecked(true);
    ui->radioButton_q5_1->setChecked(true);
    ui->radioButton_q6_1->setChecked(true);
    ui->radioButton_q7_1->setChecked(true);
    ui->textEdit->setText("A good teacher.");
}


void StackedEvaluationWidget::on_pushButton_send_clicked() {
    QStringList userContent;
    userContent << transferUserChoose(ui->buttonGroup_1->checkedButton()->text())
                << transferUserChoose(ui->buttonGroup_2->checkedButton()->text())
                << transferUserChoose(ui->buttonGroup_3->checkedButton()->text())
                << transferUserChoose(ui->buttonGroup_4->checkedButton()->text())
                << transferUserChoose(ui->buttonGroup_5->checkedButton()->text())
                << transferUserChoose(ui->buttonGroup_6->checkedButton()->text());
    if( ui->buttonGroup->checkedId()==-2) userContent << transferUserChoose(ui->buttonGroup_7->checkedButton()->text());//如果是老师评教页面
    userContent << gbkURLCoding(ui->textEdit->toPlainText());

    emit sendEvaluateInfo(userContent);
}
QString StackedEvaluationWidget::transferUserChoose(QString str)
{
    if( str=="非常符合" )  return "10_1";
    else if( str=="较符合" ) return "10_0.8";
    else if( str=="一般" ) return "10_0.6";
    else if( str=="不符合" ) return "10_0.4";
    else if( str=="很不符合" ) return "10_0.2";
    else return "10_1";

}

void StackedEvaluationWidget::on_pushButton_flush_clicked() {
    if(m_flush==0) {
        m_timer->start( 5000 );
        m_flush =1;
        ui->label_flush->setText("正在刷新");
        emit flushEvaluationInfo(true);
    }
}

void StackedEvaluationWidget::flushEvaluationInfo() {
    m_evaluationModel->select();
    if( m_flush==1 ) {
        ui->label_flush->setText("刷新完成");
        m_flush =0;
        m_timer->stop();
    }
}


void StackedEvaluationWidget::setUnaccessible()
{
    ui->label_flush->setText("刷新超时");
    m_flush =0;
}


void StackedEvaluationWidget::handleStackButtonClick(int order) {
    if(m_lastOrder!=order) {
        switch (order) {
        case -2://教师评教
            ui->label_question1->setText("问题1：老师热爱教学，用心经营教学，对所授课程有明确的目标，在教学活动中，充满\n爱心，言传身教，立德树人，传递正能量。");
            ui->label_question2->setText("问题2：该课程的教学很好地激发了我的学习兴趣并调动了我的学习积极性，我在该课程\n学习中付出了最大努力。");
            ui->label_question3->setText("问题3：老师为我提供的学习资源以及引导我们自主寻找的学习资源，对我的学习帮助很\n大。");
            ui->label_question4->setText("问题4：老师精心设计课程的成绩评定办法和过程考核环节，激励我在整个学期中都努力\n学习。");
            ui->label_question5->setText("问题5：作业和考试后，老师针对我的学习情况给予了及时且有价值的反馈，这些反馈可\n以很好的帮助我了解如何改进学习。");
            ui->label_question6->setText("问题6：我认为，该门课程选用了具有思想性、科学性、时代性等特点的优质教材。");
            ui->label_question7->setText("问题7：该门课程的总体授课效果令我很满意。");
            ui->label_question7->show();
            ui->radioButton_q7_1->show();
            ui->radioButton_q7_2->show();
            ui->radioButton_q7_3->show();
            ui->radioButton_q7_4->show();
            ui->radioButton_q7_5->show();

            ui->stackedWidget->setCurrentIndex(0);
            m_evaluationModel->setFilter("Type=1 and Semester='"+m_semester+"' and Username='"+m_username+"' ");
            m_evaluationModel->select();
            break;
        case -3://助教评教
            ui->label_question1->setText("问题1：研究生助教对你的学习是否有帮助？");
            ui->label_question2->setText("问题2：研究生助教是否认真责任？");
            ui->label_question3->setText("问题3：研究生助教是否帮助你增加了与主讲老师的沟通互动？");
            ui->label_question4->setText("问题4：研究生助教是否有利于激发你的学习兴趣？");
            ui->label_question5->setText("问题5：研究生助教与你联系的频率如何？");
            ui->label_question6->setText("问题6：研究生助教是否能清晰地解答你的疑问？");
            ui->label_question7->hide();
            ui->radioButton_q7_1->hide();
            ui->radioButton_q7_2->hide();
            ui->radioButton_q7_3->hide();
            ui->radioButton_q7_4->hide();
            ui->radioButton_q7_5->hide();

            ui->stackedWidget->setCurrentIndex(0);
            m_evaluationModel->setFilter("Type=0 and Semester='"+m_semester+"' and Username='"+m_username+"' ");
            m_evaluationModel->select();
            break;
            case -4://小学期评教
                ui->stackedWidget->setCurrentIndex(1);
                break;
            default:
                break;
            }
        }

    m_lastOrder =order;
}
