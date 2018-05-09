#include "LoginWindow.h"
#include "ui_loginwindow.h"
#include "../Network/educationsystem.h"
#include <QObject>
LoginWindow::LoginWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginWindow),m_drag(false),m_trayIcon(new QSystemTrayIcon(this)),m_usernameListWidget(new QListWidget(this)),
    m_mainWindow(nullptr)
{
    initSQL();
    ui->setupUi(this);
    m_usernameListWidget->setGeometry(0,0,0,0);
    setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    fillUsernameComboBox();
    fillSemesterComboxBox();
    createTrayIcon();

}

void LoginWindow::createTrayIcon(void) {
    //设置图标和提示语
    m_trayIcon->setIcon(QIcon(":/Resources/battery.ico"));
    m_trayIcon->setToolTip("JWCKnife");
    //设置右键菜单栏
    QMenu *trayIconMenu = new QMenu(this);
    m_normalAction = new QAction(QIcon(":Resources/menu_open.png"),"打开面板", this);
    connect(m_normalAction, SIGNAL(triggered()), this, SLOT(showWindow()));
    m_quitAction = new QAction(QIcon(":Resources/menu_close.png"),"退出", this);
    connect(m_quitAction, SIGNAL(triggered()), &m_thread, SLOT(quit()));
    connect(m_quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    trayIconMenu->addAction(m_normalAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(m_quitAction);
    m_trayIcon->setContextMenu(trayIconMenu);
    //显示提示消息与tray icon
    m_trayIcon->show();
    m_trayIcon->showMessage("Battery tells","我在右下角的托盘那里哦",QSystemTrayIcon::Information,5000);
    connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                this,SLOT(trayiconActivated(QSystemTrayIcon::ActivationReason)));
}

void LoginWindow::initSQL(void) {
    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE");
//    QSqlDatabase db=QSqlDatabase::addDatabase("SQLITECIPHER");
    db.setDatabaseName("information.db");
    db.setUserName("sa");
    db.setPassword("123");
    if(!db.open()){
        qDebug() << "数据库连接失败" << db.lastError().driverText();;
        exit(1);
    }
    m_pQuery =new QSqlQuery(db);
    SQLErrorDetect(m_pQuery,
                 "create table  if not exists Users ("
                 "Username varchar,"
                 "Password varchar,"
                 "Semester varchar,"
                 "Frequence int not null default 1 )", "创建用户表失败");
}


void LoginWindow::fillUsernameComboBox() {
    ui->comboBox_username->setEditable(true);
    ui->comboBox_username->lineEdit()->setPlaceholderText("请输入本科系统账号");//需要comboBox_username->setEditable(true)
    SQLErrorDetect(m_pQuery, "select * from Users order by frequence desc", "获取账号列表失败");
    ui->comboBox_username->setModel(m_usernameListWidget->model());
    ui->comboBox_username->setView(m_usernameListWidget);
    if( m_pQuery->next() ) {
        QString tmpUsername =m_pQuery->value(0).toString(); //不能直接进行ui->comboBox_username->setCurrentText，while会清空
        ui->lineEdit_password->setText(m_pQuery->value(1).toString());
        do {
            AccountItem *pAccountItem =new AccountItem();
            pAccountItem->setText(m_pQuery->value(0).toString());
            QListWidgetItem* widgetItem = new QListWidgetItem(m_usernameListWidget);
            m_usernameListWidget->setItemWidget(widgetItem, pAccountItem);
            connect( pAccountItem, SIGNAL(deleteAccount(QString)),this, SLOT(deleteAccount(QString)) );
            connect( pAccountItem, SIGNAL(showAccount(QString)),this, SLOT(showAccount(QString)) );
        }while(m_pQuery->next());
        ui->comboBox_username->setCurrentText(tmpUsername);
    }
}

void LoginWindow::fillSemesterComboxBox() \
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    int currentYear = currentDateTime.toString("yyyy").toInt();
    int currentMonth = currentDateTime.toString("MM").toInt();
    for(int i1=currentYear-4;i1<=currentYear+4;++i1) {
        ui->comboBox_semester->addItem(QString::number(i1)+"春");
        ui->comboBox_semester->addItem(QString::number(i1)+"秋");
    }
    if(currentMonth<=8)
        ui->comboBox_semester->setCurrentText(QString::number(currentYear)+"春");
    else
        ui->comboBox_semester->setCurrentText(QString::number(currentYear)+"秋");
}

LoginWindow::~LoginWindow()
{
//    delete m_pAccount;
    delete m_pQuery;
    QSqlDatabase db=QSqlDatabase::database();
    db.close();
    delete ui;
}

void LoginWindow::showWindow(void) {
    m_mainWindow==nullptr?this->showNormal():m_mainWindow->showNormal();
}



//下面三个鼠标响应事件是为了能够对窗口进行拖动
void LoginWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
         m_drag = true;
         m_windowPosition = event->globalPos() - this->pos();
         event->accept();
     }
}

void LoginWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (m_drag && event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - m_windowPosition);
        event->accept();
    }
}

void LoginWindow::mouseReleaseEvent(QMouseEvent *)
{
    m_drag = false;
}


void LoginWindow::on_pushButton_login_clicked()
{

    QString username =ui->comboBox_username->currentText();
    QString password =ui->lineEdit_password->text();
    QString semester =ui->comboBox_semester->currentText();
    int year =semester.left(4).toInt();
    //2017秋 =2017-2018秋  2017春=2016-2017春
    if(semester.right(1)=="春")
        semester =QString::number(year-1)+"-"+QString::number(year)+"学年春";
    else
        semester =QString::number(year)+"-"+QString::number(year+1)+"学年秋";

    //将网络对象存放到一个单独的线程中，同时在线程开始的时候进行第一次数据获取
    m_pAccount= new EducationSystem(username, password, semester);
    m_pAccount->moveToThread(&m_thread);
    connect(&m_thread,SIGNAL(started()), m_pAccount, SLOT(init()), Qt::QueuedConnection);
    m_thread.start();

    m_mainWindow =new MainWindow(m_pAccount);
    m_mainWindow->move(QPoint(this->pos().x()+this->size().width()/2-m_mainWindow->size().width()/2, this->pos().y()+this->size().height()/2-m_mainWindow->size().height()/2));
    m_mainWindow->show();
    this->close();
}

//处理子控件QCombobox发出的show变更信号
void LoginWindow::showAccount(QString username) {
    SQLErrorDetect(m_pQuery, "select password from users where username='"+username+"'", "更改选择框账号时查询密码出错");
    m_pQuery->next();
    ui->comboBox_username->setCurrentText(username);
    ui->lineEdit_password->setText(m_pQuery->value(0).toString());
    ui->comboBox_username->hidePopup();
}
//处理子控件QCombobox发出的delelte删除信号
void LoginWindow::deleteAccount(QString username) {
    SQLErrorDetect(m_pQuery, "delete from users where username='"+username+"'", "删除账号时出错");
    QList<QListWidgetItem*> ItemPointList =m_usernameListWidget->findItems(".*",Qt::MatchRegExp);
    for(int i1=0; i1<ItemPointList.size(); ++i1) {
        AccountItem *pAccountItem =static_cast<AccountItem *>( m_usernameListWidget->itemWidget(ItemPointList[i1]));
        if(pAccountItem->text()==username) {
            delete ItemPointList[i1];
            ItemPointList.removeAt(i1);
            break;
        }
    }

    if(!ItemPointList.isEmpty()) {
        QString username=static_cast<AccountItem *>(m_usernameListWidget->itemWidget(ItemPointList.first()))->text();
        SQLErrorDetect(m_pQuery, "select password from users where username='"+username+"'", "因删除更改选择框账号时查询密码出错");
        m_pQuery->next();
        ui->comboBox_username->setCurrentText(username);
        ui->lineEdit_password->setText(m_pQuery->value(0).toString());
        ui->comboBox_username->showPopup();
    }else{
        ui->lineEdit_password->setText("");
        ui->comboBox_username->hidePopup();
    }

}


void LoginWindow::trayiconActivated(QSystemTrayIcon::ActivationReason reason) {
    switch (reason)
    {
        case QSystemTrayIcon::Trigger:
            //单击托盘图标
        case QSystemTrayIcon::DoubleClick:
            //双击托盘图标
        case QSystemTrayIcon::MiddleClick:
            //鼠标中间的滚轮点击
            if(m_mainWindow==nullptr) {
                this->showNormal();
                this->raise();
            }else {
                m_mainWindow->showNormal();
                m_mainWindow->raise();
            }
            break;
        default:
            break;
    }

}
