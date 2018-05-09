#ifndef EDUCATIONSYSTEM_H
#define EDUCATIONSYSTEM_H

#include <QTextCodec>
#include "pagelist.h"
//Network
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkCookieJar>
#include <QNetworkReply>
//QSQLITE
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
//exit
#include <QCoreApplication>
//icon
#include <QSystemTrayIcon>
//Sync reply for login()
#include <QEventLoop>
//splash when wait
//#include <QPixmap>
//#include <QSplashScreen>
//prompt
#include <QMessageBox>

QList<QString> findAll(const QString text, const QString regexp, int capPos, const QString removeRules="");
void sleep(int ms);
bool SQLErrorDetect (QSqlQuery *pQuery, const QString strCommand, const char *info);
bool SQLErrorDetectForPrepare (QSqlQuery *pQuery, const char *info);
extern QTextCodec *gbk;
QString gbkURLCoding(QString str);

class EducationSystem: public QNetworkAccessManager
{
    Q_OBJECT
public:
    EducationSystem(const QString u, const QString p, const QString s);
    ~EducationSystem(void);
    QString getSemester(void);
    QString getUsername(void);



private:
    bool login(void);
    void logout(void);
    void handleError(QNetworkRequest request); //处理请求服务器时产生的错误
    void recordPlanCourses(QString str);
    void recordGrades(QString str);
    bool recordClassSchedule(QString &str);
    void recordEvaluateInfo(QString &str);
    void recordEvaluationValueList(QString &str);
    void recordAllCourses(QString &str);
    void recordAllCourses2(QString &str);
    void recordSchoolPublicCourses(QString &str);
    void recordMaterialInfo(QString &str);
    bool recordExamInfo(QString &str);
    void afterSelectClass(QString &url, QString &str);
    void afterDeleteClass(QString &url, QString &str);


signals:
    void unaccessible();
    void sendClassResult(QString);
    void evaluationResult(QString);
    void selectClassResult(QString);
    void deleteClassResult(QString);
    void timerMessage(QString);
    void selectMaterialResult(QString);
    void getSchoolPublicCoursesError(QString);
    void needEvaluateError();
    void haveGetClassSchedule();
    void haveGetMaterialInfo();
    void haveGetExamInfo();
    void haveGetEvaluation();//爬取评教信息，控制界面的刷新
    void haveGetEvaluationAfterSend();//发送完毕根据结果进行数据库刷新与准备失败提示窗口
    void haveGetGrades();
    void haveGetSchoolPublicCourses();
    void beginRecordAllCourses();
    void haveGetAllCourses();
public slots:
    void init(void);
    void getGrades(void);
    void sendClassInfo(void);                       //发送选课、删课信息
    void getPlanCourses(const bool force=false);    //获取方案完成情况
    void getClassSchedule(const bool force=false);  //获取课表
    void getEvaluateInfo(const bool force=false);   //获取评教信息
    void sendEvaluateInfo(QStringList userContent); //发送评教信息
    void getAllCourses(void);                       //获取选课时候的全部课程(还没有被使用)
    void getAllCourses2(void);                      //获取选课时候的全部课程(还没有被使用)
    void getSchoolPublicCourses(void);              //获取选课时候的校公选课（未使用）
    void getMaterialInfo(void);                     //获取本学期教材信息
    void sendMaterialInfo(void);                    //发送自己选择的教材信息
    void getExamInfo(const bool force);             //获取考试相关信息
    void sendSelectClassInfo(void);
    void sendDeleteClassInfo(void);


private slots:
    void replyComplished(QNetworkReply *pReply);    //处理除了login与logout意外所有的http响应包
    void setUnaccessible(void);


private:
    //Network operator
    bool m_accessible;
    QString m_BaseUrl;
    QString m_username;
    QString m_password;
    QString m_semester;
    QNetworkRequest m_request;
    QNetworkCookieJar *m_pCookieJar;
    QStringList m_evaluationValue;
    QString m_evaluationResult;QString m_lastbpr,m_lastpgnr;
    bool m_bSendClass;
    QTimer *m_timer;
    //SQL operator
    QSqlQuery *m_pQuery;
};

#endif // EDUCATIONSYSTEM_H
