#include "educationsystem.h"
#include <QTimer>

//和主界面直接由交互吗，不然就会需要重新登录
/*500异常来自 javax.servlet.jsp.JspException: ServletException in '/WEB-INF/JSP/xk/inforReFresh.jsp':
* 不是我想象中的内存不够导致session被替换，因为直接刷新还是我的名字
* */

// /xkAction.do?actionType=7 删除课程
//给出提醒于收藏页 *:....，正选的时候调整选课优先级
//全部课程   共152页 第1页
//450 包  选课未成功信息  /xkAction.do?actionType=16
//84包 kch=&kcm=&actionType=3&pageNumber=-1

//81包 自由搜索  kch=&cxkxh=&kcm=&skjs=&kkxsjc=&skxq=&skjc=&pageNumber=-2&preActionType=1&actionType=5
//无效参数：preActionType=3  kch=&cxkxh=&kcm=&skjs=&kkxsjc=&skxq=&skjc=&pageNumber=-2&actionType=5
EducationSystem::EducationSystem(const QString u, const QString p, const QString s):\
    m_BaseUrl("http://202.115.47.141"),m_username(u),m_password(p),m_semester(s), m_pCookieJar(new QNetworkCookieJar),
    m_pQuery(new QSqlQuery(QSqlDatabase::database())),
    m_bSendClass(true),m_accessible(false),m_timer(nullptr)
{
    setCookieJar(m_pCookieJar);
    m_request.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 AppleWebKit/537.36 (KHTML, like Gecko) Chrome/59.0.3053.3 Safari/537.36");
    m_request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    connect(this, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyComplished(QNetworkReply*)), Qt::UniqueConnection);
    SQLErrorDetect(m_pQuery,
                "create table  if not exists ClassSchedule ("  //去除培养方案（全部都是自己专业的）
                "Semester varchar,"         //学期
                "Username varchar,"         //账号
                "State tinyint,"            //是否真实存在课表中1（还是自己暂时添加的课程0，还是在回收站里面2）
                "CourseId varchar,"         //课程号
                "CourseName varchar,"       //课程名字
                "CourseAttribute varchar,"  //课程属性 任选 or 必修
                "ClassId varchar,"          //课序号
                "Credir varchar,"           //学分
                "TypeofExam varchar,"       //考试类型 考试 or 考查 or 空着
                "Teacher varchar,"          //教师
                "WayofAttending varchar,"   //修读方式 正常 or 重修
                "SelectWay varchar,"        //选中方式 拟选  抽中、置入、选中
                "TeachingWeek varchar,"     //周次
                "TeachingDay varchar,"      //星期
                "TeachingSession varchar,"  //节次
                "Campus varchar,"           //校区
                "TeachingBuilding varchar," //教学楼
                "TeachingRoom varchar )"    //教室
                ,"创建课表数据库失败");
    SQLErrorDetect(m_pQuery,
                "create table  if not exists Courses (" //去除了、选课模式、选课控制
                "Semester varchar,"         //学期
                "IsSchoolPublicCourse tinyint," //判断是不是校公选
                "IfSend tinyint,"            //判断是不是选择发送
                "IfStar tinyint,"            //判断是不是选择收藏
                "ProfessionofCourse varchar," //开课系
                "CourseId varchar,"         //课程号
                "CourseName varchar,"       //课程名字
                "ClassId varchar,"          //课序号
                "Credir varchar,"           //学分
                "TypeofExam varchar,"       //考试类型 考试 or 考查 or 空着
                "Teacher varchar,"          //教师
                "ClassRemaining int,"       //课余量   只有这个是int
                "ClassLimit varchar,"       //选课限制
                "TeachingWeek varchar,"     //周次
                "TeachingDay varchar,"      //星期
                "TeachingSession varchar,"  //节次
                "Campus varchar,"           //校区
                "TeachingBuilding varchar," //教学楼
                "TeachingRoom varchar )"    //教室
                ,"创建公选课数据库失败");
    SQLErrorDetect(m_pQuery,
                "create table  if not exists Materials (" //教材信息
                "Semester varchar,"         //学期
                "Username varchar,"         //账号
                "IfChoose tinyint,"         //是否勾选
                "CourseName varchar,"       //对应的课程名字
                "Info varchar)"             //用于选教材的发包信息
                ,"创建教材信息数据库失败");
    SQLErrorDetect(m_pQuery,
                "create table  if not exists Evaluation (" //评教信息
                "Semester varchar,"         //学期
                "Username varchar,"         //账号
                "IfSend tinyint,"           //判断是不是选择发送（不发送、发送、已评 三种状态）
                "Person varchar,"           //被评人名字
                "Type tinyint,"             //1表示老师 0表示助教
                "CourseName varchar,"       //课程名字（评估内容）
                "wjbm varchar,"             //问卷编码
                "bpr varchar,"              //被评人系统内账号
                "pgnr varchar)"             //评估编码
                ,"创建评教信息数据库失败");
    SQLErrorDetect(m_pQuery,
                "create table  if not exists  PlanCourses(" //方案课程
                "Username varchar,"         //账号
                "Attribute varchar,"        //课程属性：公选必修 专业基础必修  专业基础选修 专业必修  专业选修  实践必修  实践选修 跨专业选修 中华文化
                "IfAttended tinyint,"       //是否上过课程
                "CourseId varchar,"         //课程号
                "CourseName varchar,"       //课程名字
                "Credir varchar )"          //学分
                ,"创建方案课程数据库失败");
    SQLErrorDetect(m_pQuery,
                "create table  if not exists Grades(" //方案课程
                "Username varchar,"         //用户名
                "IfChecked tinyint,"        //是否选中（进行绩点计算）
                "Semester varchar,"         //学期，比如 2015-2016学年秋
                "CourseId varchar,"         //课程号
                "ClassId varchar,"          //课序号
                "CourseName varchar,"       //课程名字
                "Credir varchar,"           //学分
                "Attribute varchar,"        //课程属性：必修、选修、任选
                "Grade varchar,"            //成绩
                "Reason varchar )"          //未通过原因
                ,"创建成绩数据库失败");
    SQLErrorDetect(m_pQuery,
                "create table  if not exists Exams(" //考试
                "Semester varchar,"         //学期
                "Username varchar,"         //账号
                "Attribute varchar,"         //考试名（属性）
                "Campus varchar,"           //校区
                "Building varchar,"         //教学楼
                "Room varchar,"            //教室
                "CourseName varchar,"       //课程名字
                "Week varchar,"             //周次
                "Day varchar,"              //星期
                "Date varchar,"             //日期
                "Time varchar,"             //时间
                "PositionId varchar)"       //座位号
                ,"创建考试信息数据库失败");
    SQLErrorDetect(m_pQuery,
                "create table  if not exists PE_exam(" //考试
                "question varchar(150),"    //问题
                "A integer,"                //A选项
                "B integer,"                //B选项
                "C integer,"                //C选项
                "D integer,"                //D选项
                "answer varchar(35) );"     //回答
                ,"创建体育理论信息数据库失败");

}


EducationSystem::~EducationSystem(void) {
    delete m_pQuery;
    delete m_pCookieJar;

}


QString EducationSystem::getSemester(void) {
    return m_semester;
}

QString EducationSystem::getUsername(void) {
    return m_username;
}



//MainWindow的构造函数中执行了这个测试函数
void EducationSystem::init(void) {


    //因为是同步，所以如果万一网页反应了，程序就会卡着，这里应该进入动画并设置计时（封装成一个函数）
    if( login() ) {
        SQLErrorDetect(m_pQuery,"vacuum", "减小数据库所占空间失败");
        //account handle
        SQLErrorDetect(m_pQuery, "select * from Users where Username='"+m_username+"'", "用户名匹配超找失败");
        if( !m_pQuery->next() ) {
            SQLErrorDetect(m_pQuery,"insert into Users values('"+m_username+"','"+m_password+"','"+m_semester+"',1)", "增加用户账户记录失败");
        }else {
            SQLErrorDetect(m_pQuery,"update Users set Password='"+m_password+"',Semester='"+m_semester+"',Frequence=Frequence+1 where Username='"+m_username+"'", "跟新用户账户数据失败");
        }
        //information handle
        getClassSchedule(false);
        getPlanCourses(false);
        getExamInfo(false);
        logout();

    }else {
        qDebug() << "login invalid";

    }
//    moveToThread();
}


bool EducationSystem::login(void) {
    m_accessible =true;
    m_request.setUrl(QUrl(m_BaseUrl+"/loginAction.do"));
    QByteArray data;
    data.append("zjh="+m_username);
    data.append("&mm="+m_password);

    QNetworkReply *pReply = post(m_request, data);
    QEventLoop loop;
    connect(pReply, SIGNAL(finished()), &loop, SLOT(quit()));


    if(!m_accessible) {
        QTimer *timer = new QTimer();
        connect( timer, SIGNAL(timeout()), &loop, SLOT(quit()) );
        connect( timer, SIGNAL(timeout()), this, SLOT(setUnaccessible()) );
        timer->start( 8000 ); // 5秒单触发定时器
        loop.exec();
        timer->stop();
        delete timer;
    }

    return pReply->readAll().size()<1000;
}

void EducationSystem::logout(void) {
    m_request.setUrl(QUrl(m_BaseUrl+"/logout.do"));
    QByteArray data;
    data.append("loginType=platformLogin");

    QNetworkReply *pReply = post(m_request, data);
    QEventLoop loop;
    connect(pReply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    return;
}

//获取课表
void EducationSystem::getClassSchedule(const bool force) {
    m_request.setUrl(QUrl(m_BaseUrl+"/xkAction.do?actionType=6"));
    if( force==true && SQLErrorDetect(m_pQuery, "delete from ClassSchedule where Semester='"+m_semester+"' and Username='"+m_username+"';", "删除课程表数据失败 in getClassSchedule()") ) {
        get(m_request);
        return;
    }else {
        if( SQLErrorDetect(m_pQuery, "select count(*) from ClassSchedule where Semester='"+m_semester+"' and Username='"+m_username+"';", "探测课程数据表size失败 in getClassSchedule()") ) {
            m_pQuery->next();
            if(m_pQuery->value(0).toInt()==0 )
                get(m_request);
        }
    }

}


//记录课表
bool EducationSystem::recordClassSchedule(QString &str){
    QList<QString> firstResult =findAll(str, "<tr class=(\")?odd(\")? onMouseOut=(\")?this.className='even';(\")? onMouseOver=(\")?this.className='evenfocus';(\")?>(.*)</tr>", 7, "(&nbsp;|[\r\n\t]|<td (rowspan=\"\\d\" )?align=\"center\">.*</td>|<td( rowspan=\"\\d\" )?>.*方案</td>)");
    if( firstResult.size()==0 ) return false;
    QList<QString> lastFullResult;
    for(int i1=0;i1<firstResult.size();++i1) {
        m_pQuery->prepare("insert into ClassSchedule values(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
        m_pQuery->bindValue(0,m_semester);
        m_pQuery->bindValue(1,m_username);
        m_pQuery->bindValue(2,1);
        QList<QString> endResult =findAll(firstResult[i1], "<td.*>(.*)</td>", 1, " ");
        int size =endResult.size();
        for(int i2=0;i2<size;++i2) {
            if(size==15) {
                switch(i2){  //我的显示view对jwc的显示view在顺序上进行了一定的调整
                    case 2: //课序号
                        m_pQuery->bindValue(6,endResult.at(i2));
                        break;
                    case 3: //学分
                        m_pQuery->bindValue(7,endResult.at(i2));
                        break;
                    case 4://课程属性 任选 or 必修
                        m_pQuery->bindValue(5,endResult.at(i2));
                        break;
                    default:
                        m_pQuery->bindValue(i2+3,endResult.at(i2));
                }
                lastFullResult =endResult;
            }else if( size==6 ) { //多行课程 ("8-17周上", "5", "1~2", "江安", "实验室", "二基A二楼")
                m_pQuery->bindValue(i2+12,endResult.at(i2));
            }
        }
        if( size==6 ) {
            m_pQuery->bindValue(3,lastFullResult.at(0));
            m_pQuery->bindValue(4,lastFullResult.at(1));
            m_pQuery->bindValue(5,lastFullResult.at(4));
            m_pQuery->bindValue(6,lastFullResult.at(2));
            m_pQuery->bindValue(7,lastFullResult.at(3));
            m_pQuery->bindValue(8,lastFullResult.at(5));
            m_pQuery->bindValue(9,lastFullResult.at(6));
            m_pQuery->bindValue(10,lastFullResult.at(7));
            m_pQuery->bindValue(11,lastFullResult.at(8));
        }
        SQLErrorDetectForPrepare(m_pQuery, "插入课程表数据执行失败 in recordClassSchedule()");
    }


    return true;
}




void EducationSystem::getGrades(void) {
    m_request.setUrl(QUrl(m_BaseUrl+"/gradeLnAllAction.do?type=ln&oper=qbinfo"));
    get(m_request);
}
void EducationSystem::recordGrades(QString str) {
    qDebug() << "enter recordGrades";
    QStringList strList = str.split("<a name="),semesterList;
    SQLErrorDetect(m_pQuery, "select Semester from Grades where Username='"+m_username+"' group by Semester;", "获取Grades中所有的Semester");
    while( m_pQuery->next() )
        semesterList.append(m_pQuery->value(0).toString());
    for(int i1=1; i1<strList.size(); ++i1) {
        QString semester = strList[i1].mid(1,12);
        if( !semesterList.contains(semester) ){
            QList<QString> firstResult = findAll(strList[i1], "<tr class=(\")?odd(\")? onMouseOut=(\")?this.className='even';(\")? onMouseOver=(\")?this.className='evenfocus';(\")?>(.*)</tr>", 7, "(&nbsp|[;\r\n\t])");
            for( auto sTmp : firstResult ) {
                QList<QString> endResult =findAll(sTmp, "<td align=(\")?center(\")?>(.*)</td>", 3, "<p align=(\")?center(\")?>|</P>|[ \r\n\t]");
                endResult.removeAt(3);
                m_pQuery->prepare("insert into Grades values(?,?,?,?,?,?,?,?,?,?)");
                m_pQuery->addBindValue(m_username);
                m_pQuery->addBindValue(0);
                m_pQuery->addBindValue(semester);
                for( auto str : endResult )
                    m_pQuery->addBindValue(str);
                if(endResult.size()==5) m_pQuery->addBindValue(""); //有时候部分课程成绩还没有显示出来
                if(endResult.size()==6) m_pQuery->addBindValue("");

                SQLErrorDetectForPrepare(m_pQuery, "插入成绩表数据执行失败 in recordGrades(QString str)");
            }
        }else {
            continue;
        }
    }

}


//如果是查看自己的学情况：/gradeLnAllAction.do?type=ln&oper=lnfaqkxx&fajhh=2806
void EducationSystem::getPlanCourses(const bool force) {
    m_request.setUrl(QUrl(m_BaseUrl+"/gradeLnAllAction.do?type=ln&oper=lnfaqk&flag=zx"));
    if( force==true && SQLErrorDetect(m_pQuery, "delete from PlanCourses where Username='"+m_username+"';", "删除课程表数据失败 in getPlanCourses()") ) {
        get(m_request);
        return;
    }else {
        if( SQLErrorDetect(m_pQuery, "select count(*) from PlanCourses where Username='"+m_username+"';", "探测课程数据表size失败 in getPlanCourses()") ) {
            m_pQuery->next();
            if(m_pQuery->value(0).toInt()==0 )
                get(m_request);
        }
    }

}
void EducationSystem::recordPlanCourses(QString str) {
    str =findAll(str,"tree\\.config\\.useCookies=false;(.*)document.write\\(tree\\);", 1, "[\r\n\t]|<!--(.*)-->")[0];
    QString attribute="";
    for(auto firstResult : str.split(QRegExp("(\\);)?tree.add\\("),QString::SkipEmptyParts) ) {
        QString information =firstResult.split("'")[1];
        if(information.left(1)=="[") {//格式：[304112010]新生研讨课[1](必修,88.0(2016-01-14))
            QStringList result = information.split(QRegExp("\\[|\\]"),QString::SkipEmptyParts);
            int ifAttended =result.size()>2;
            if(ifAttended==1)
                SQLErrorDetect(m_pQuery,"insert into PlanCourses values('"+m_username+"',"
                               "'"+attribute+"',"                   //Attribute
                               +QString::number(ifAttended)+"," //IfAttended
                               "'"+result[0]+"',"                   //CourseId
                               "'"+result[1]+"',"                   //CourseName
                               "'"+result[2]+"')",                  //Credir
                               "插入PlanCourses失败 in EducationSystem::recordPlanCourses(QString str)");
            else
                SQLErrorDetect(m_pQuery,"insert into PlanCourses values('"+m_username+"',"
                               "'"+attribute+"',"                   //Attribute
                               +QString::number(ifAttended)+"," //IfAttended
                               "'"+result[0]+"',"                   //CourseId
                               "'"+result[1]+"',"                   //CourseName
                               "'')",                           //Credir
                               "插入PlanCourses失败 in EducationSystem::recordPlanCourses(QString str)");
        }else { //格式：公共课(最低修读学分:33,通过学分:31.0,已修课程门数:22,已通过课程门数:22,未通过课程门数:0)
            QString str =information.split("(")[0];
            if(str=="必修" || str=="选修")
                attribute +=str;
            else
                attribute =str;
        }
    }

}



//发现  不需要进行登录 还可以使用GET方法访问actionType=2&pageNumber=xxxx，但是没有actionType=1那样可以对显示的列进行调整
// 在网络通畅的情况下，需要1分钟40秒的时间，数据库会增加7500行数据。
void EducationSystem::getAllCourses(void) {
        m_request.setUrl(QUrl(m_BaseUrl+"/courseSearchAction.do?pageNumber=0&pageSize=10000\
&showColumn=kkxsjc%23%BF%AA%BF%CE%CF%B5\
&showColumn=kch%23%BF%CE%B3%CC%BA%C5\
&showColumn=kcm%23%BF%CE%B3%CC%C3%FB\
&showColumn=kxh%23%BF%CE%D0%F2%BA%C5\
&showColumn=xf%23%D1%A7%B7%D6\
&showColumn=kslxmc%23%BF%BC%CA%D4%C0%E0%D0%CD\
&showColumn=skjs%23%BD%CC%CA%A6\
&showColumn=bkskrl%23%BF%CE%C8%DD%C1%BF\
&showColumn=xkxzsm%23%D1%A1%BF%CE%CF%DE%D6%C6%CB%B5%C3%F7\
&showColumn=zcsm%23%D6%DC%B4%CE\
&showColumn=skxq%23%D0%C7%C6%DA\
&showColumn=skjc%23%BD%DA%B4%CE\
&showColumn=xqm%23%D0%A3%C7%F8\
&showColumn=jxlm%23%BD%CC%D1%A7%C2%A5\
&showColumn=jasm%23%BD%CC%CA%D2\
&actionType=1"));
        post(m_request, QByteArray());
}

void EducationSystem::recordAllCourses(QString &str) {
    emit beginRecordAllCourses();
    SQLErrorDetect(m_pQuery, "delete from Courses","删除所有可选课程，重新插入 in EducationSystem::recordTest(QString str)");
    QList<QString> firstResult =findAll(str, "<tr class=(\")?odd(\")? onMouseOut=(\")?this.className='even';(\")? onMouseOver=(\")?this.className='evenfocus';(\")?>(.*)</tr>", 7, "(&nbsp|[; \r\n\t]|<td align=(\")?center(\")?>.*</td>)");
    for(auto extract : firstResult) {
        QList<QString> endResult =findAll(extract, "<td.*>(.*)</td>", 1);
        m_pQuery->prepare("insert into Courses values(?,0,0,0,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
        m_pQuery->addBindValue(m_semester);
        for(int i1=0;i1<endResult.size();++i1)  //对size进行一个判断
            m_pQuery->addBindValue(endResult[i1]);
        SQLErrorDetectForPrepare(m_pQuery, "插入Courses表数据失败");
    }

}

//获取数据的第二个接口，这个接口只有在真正选课的时候才会开放，不推荐使用
void EducationSystem::getAllCourses2(void) {
    //需要发一个探测是否开着选课的包，并通过这个包的返回进行判断是否继续?
     QString url=m_BaseUrl+"/xkAction.do?actionType=5&kch=&cxkxh=&kcm=&skjs=&kkxsjc=&skxq=&skjc=&pageNumber=-2";
     m_request.setUrl(url);
     get(m_request);


   for(int i1=1;i1<153;++i1) {
       url =m_BaseUrl+"/xkAction.do?actionType=5&pageNumber="+QString::number(i1);
       m_request.setUrl(url);
       get(m_request);
   }
}
//记录全部课程
void EducationSystem::recordAllCourses2(QString &str) {
    qDebug() << "enter recordAllCourses2()";

    QList<QString> firstResult =findAll(str, "<tr class=(\")?odd(\")? onMouseOut=(\")?this.className='even';(\")? onMouseOver=(\")?this.className='evenfocus';(\")?>(.*)</tr>", 7, "(&nbsp|[; \r\n\t]|<td rowspan=(\")?\\d(\")? align=(\")?center(\")?>.*<input.*</td>|</?a.*>|</?div.*>|</?span.*>)");
    for(auto extract : firstResult) {
        m_pQuery->prepare("insert into Courses values(?,0,0,0,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
        m_pQuery->addBindValue(m_semester);
        QList<QString> endResult =findAll(extract, "<td.*>(.*)</td>", 1);
        for(int i1=0;i1<endResult.size();++i1)  //对size进行一个判断
            if(i1<17) {
                switch(i1){  //我的显示view对jwc的显示view在顺序上进行了一定的调整
                    case 8: //略过 选课模式
                    case 9: //略过 选课控制
                        continue;
                    default:
                    m_pQuery->addBindValue(endResult.at(i1));
                }
            }else if(i1==17 || i1==24 || i1==31){
                m_pQuery->addBindValue(endResult.at(i1));
                SQLErrorDetectForPrepare(m_pQuery, "插入课程表数据执行失败 in recordAllCourses2()");
                m_pQuery->prepare("insert into Courses values(?,0,0,0,0,0,0,0,0,0,0,0,0,?,?,?,?,?,?,?)");
                m_pQuery->addBindValue(m_semester);
            }else {
                m_pQuery->addBindValue(endResult.at(i1));
            }
    }

}


/* 获取校公选
 * 专门使用了超出范围的页数查询，在对应的record函数中如果是重复的数据页面就舍弃。
 */
void EducationSystem::getSchoolPublicCourses(void) {
    SQLErrorDetect(m_pQuery, "select count(*) from Courses where Semester='"+m_semester+"';", "探测Courses表是否有数据 in getSchoolPublicCourses()");
    if( m_pQuery->next() && m_pQuery->value(0).toInt()==0 ) {emit getSchoolPublicCoursesError("请先完成全部课程下的刷新，点击那个按钮以后大约3分钟时间才会得到全部课程数据"); return;}

    QEventLoop loop;QNetworkReply *pReply;
    QString url=m_BaseUrl+"/xkAction.do?actionType=-1";
    m_request.setUrl(url);
    pReply = get(m_request);
    connect(pReply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    for(int i1=1;i1<10;++i1) {
        url=m_BaseUrl+"/xkAction.do?actionType=3&pageNumber="+QString::number(i1);
        m_request.setUrl(url);
        get(m_request);
    }

}


//记录校公选
void EducationSystem::recordSchoolPublicCourses(QString &str) {
    qDebug() << "enter recordSchoolPublicCourses()";
    QList<QString> firstResult =findAll(str, "<tr class=(\")?odd(\")? onMouseOut=(\")?this.className='even';(\")? onMouseOver=(\")?this.className='evenfocus';(\")?>(.*)</tr>", 7, "(&nbsp;|[ \r\n\t])");

    if( firstResult.size()>0)  {
        //查重判断
        QList<QString> endResult =findAll(firstResult[0], "<td.*>(.*)</td>", 1);
        SQLErrorDetect(m_pQuery, "select IsSchoolPublicCourse from Courses where CourseId='"+endResult[2]+"' and ClassId='"+endResult[4]+"' and Semester='"+m_semester+"';", "查询是否已经跟新过公选课课表失败 in recordSchoolPublicElectiveCourses()");
        if( m_pQuery->value(0).toInt()==1 ) return;
        for(int i1=0; i1<firstResult.size(); ++i1) {
            endResult =findAll(firstResult[i1], "<td.*>(.*)</td>", 1);
            m_pQuery->prepare("update Courses set IsSchoolPublicCourse=1 where CourseId=? and ClassId=? and Semester='"+m_semester+"';");
            m_pQuery->bindValue(0,endResult[2]);
            m_pQuery->bindValue(1,endResult[4]);
            SQLErrorDetectForPrepare(m_pQuery, "插入课程表数据执行失败 in recordSchoolPublicCourses()");
        }

    }
}


void EducationSystem::sendClassInfo(void) {
    //开启弹窗
    m_bSendClass =true;
    //删课、选课
    sendDeleteClassInfo();
    sendSelectClassInfo();
}

void EducationSystem::sendSelectClassInfo(void) {
    QSqlQuery *query=new QSqlQuery(QSqlDatabase::database());
    SQLErrorDetect(query, "select * from ClassSchedule where State=0 and Semester='"+m_semester+"' and Username='"+m_username+"'","从ClassSchedule数据库中选取要发送的选课信息失败 in EducationSystem::sendSelectClassInfo(void)");
    QEventLoop loop;QNetworkReply *pReply;
    if( query->next() ) {
        do{
            qDebug() << query->value("CourseId").toString() << query->value("ClassId").toString() << query->value("State");
            m_request.setUrl(m_BaseUrl+"/xkAction.do?actionType=-1");
            pReply = get(m_request);
            connect(pReply, SIGNAL(finished()), &loop, SLOT(quit()));
            loop.exec();
            m_request.setUrl(m_BaseUrl+"/xkAction.do?pageNumber=-2&actionType=5&preActionType=1&kcm=&skjs=&kkxsjc=&skxq=&skjc=&&kch="+query->value("CourseId").toString()+"&cxkxh="+query->value("ClassId").toString());
            pReply = get(m_request);
            connect(pReply, SIGNAL(finished()), &loop, SLOT(quit()));
            loop.exec();
            m_request.setUrl(QUrl(m_BaseUrl+"/xkAction.do?actionType=9&kcId="+query->value("CourseId").toString()+"_"+query->value("ClassId").toString()+"&preActionType=5"));
            pReply = post(m_request,QByteArray());
            connect(pReply, SIGNAL(finished()), &loop, SLOT(quit()));
            loop.exec();
        }while( query->next() );
    }else if(m_timer!=nullptr) {
        m_timer->stop(); delete m_timer; m_timer=nullptr;
        emit timerMessage("没有任何目标，后台程序已经停止");
    }
    delete query;
}
void EducationSystem::sendDeleteClassInfo(void) {
    SQLErrorDetect(m_pQuery, "select CourseId from ClassSchedule where State=2 and Semester='"+m_semester+"' and Username='"+m_username+"'","从ClassSchedule数据库中选取要发送的选课信息失败 in EducationSystem::sendDeleteClassInfo(void)");
    while( m_pQuery->next() ) {
        m_request.setUrl(QUrl(m_BaseUrl+"/xkAction.do?actionType=10&kcId="+m_pQuery->value(0).toString()));
        get(m_request);
    }

}
void EducationSystem::afterSelectClass(QString &url, QString &str) {
    QStringList firstResult = findAll(str,"<strong><font color=\"#990000\">(.*)</font></strong>",1);
    if(firstResult.size()==0) qDebug() << str;
    QString result =firstResult[0];
    QRegExp rx("[=&]"); QString first =url.split(rx,QString::SkipEmptyParts)[3];
    QString second =first.split("_",QString::SkipEmptyParts)[0];

    SQLErrorDetect(m_pQuery, "select CourseName from ClassSchedule where CourseId='"+second+"' and Semester='"+m_semester+"' and Username='"+m_username+"'", "根据选课结果课程号找到目标课程名字 in EducationSystem::afterSelectClass(QString &url, QString &str)");
    m_pQuery->next();
    if(result=="选课成功！") {
        emit selectClassResult(m_pQuery->value(0).toString()+" 选课成功");
        SQLErrorDetect(m_pQuery, "update ClassSchedule set State=1 where CourseId='"+second+"' and Semester='"+m_semester+"' and Username='"+m_username+"'", "选课成功后改变State失败 in EducationSystem::afterSelectClass(QString &url, QString &str)");
    }else if(result.contains("你已经选择了")) {  //返回示例：你已经选择了课程大学英语（阅读与翻译）-2[152]
        emit selectClassResult(m_pQuery->value(0).toString()+" 已经选择");
    }else if(result.contains("没有课余量")) {    //返回示例：你选择的课程俄罗斯政治与外交[01]没有课余量
        if(m_timer==nullptr) {
            m_timer =new QTimer();
            m_timer->setSingleShot(false);
            m_timer->setInterval(3000);
            connect(m_timer, SIGNAL(timeout()) , this, SLOT(sendSelectClassInfo()));
            emit timerMessage(m_pQuery->value(0).toString()+"没有课余量，开启后台运行自助选课，窗口可以关闭，右下角程序不要退出");
        }
        m_timer->start();
    }else{ //返回示例：对不起,课程投资银行理论与实践01和其他课程的上课时间冲突
        emit selectClassResult(m_pQuery->value(0).toString()+" 选课失败，请刷新课表，检查课程冲突情况");
    }

}

void EducationSystem::afterDeleteClass(QString &url, QString &str) {
    QRegExp rx("[=&]"); QString courseId =url.split(rx,QString::SkipEmptyParts).last();
    if(str.size()>10000) {
        SQLErrorDetect(m_pQuery, "select CourseName from ClassSchedule where CourseId='"+courseId+"' and Semester='"+m_semester+"' and Username='"+m_username+"'","查询被删除课程失败 in EducationSystem::afterDeleteClass(QString str)");
        m_pQuery->next();QString courseName =m_pQuery->value(0).toString();
        SQLErrorDetect(m_pQuery, "delete from ClassSchedule where CourseId='"+courseId+"' and Semester='"+m_semester+"' and Username='"+m_username+"'","删除课程失败 in EducationSystem::afterDeleteClass(QString str)");
        emit deleteClassResult("成功删除 "+courseName);
    }
}

void EducationSystem::getEvaluateInfo(const bool force) {
    m_request.setUrl(QUrl(m_BaseUrl+"/jxpgXsAction.do?oper=listWj&pageSize=100"));
    if( force==true && SQLErrorDetect(m_pQuery, "delete from Evaluation where Semester='"+m_semester+"' and Username='"+m_username+"';", "删除考试安排表数据失败 in getEvaluateInfo()") ) {
        get(m_request);
        return;
    }else {
        if( SQLErrorDetect(m_pQuery, "select count(*) from Evaluation where Semester='"+m_semester+"' and Username='"+m_username+"';", "探测考试安排表size失败 in getEvaluateInfo()") ) {
            if( m_pQuery->next() && m_pQuery->value(0).toInt()==0 )
                get(m_request);
        }
    }

}

void EducationSystem::recordEvaluateInfo(QString &str) {
    qDebug() << "recordEvaluateInfo";
    QList<QString> firstResult =findAll(str, "<tr class=(\")?odd(\")? onMouseOut=(\")?this.className='even';(\")?(\\s)*onMouseOver=(\")?this.className='evenfocus';(\")?>(.*)</tr>", 8, "(&nbsp;|[\r\n\t])");
    for(auto extract : firstResult ){
        QList<QString> secondResult =findAll(extract, "<td.*>(.*)</td>", 1);
        int IfSend =0;
        if(secondResult[3]=="否") IfSend =0;
        else IfSend =2;
        QStringList result =findAll(secondResult[4],"<img name=\"(.*)\"",1)[0].split("#@");

        if( SQLErrorDetect(m_pQuery, "select count(*) from Evaluation where Semester='"+m_semester+"' and Username='"+m_username+"' and bpr='"+result[1]+"' and pgnr='"+result[5]+"';", "探测考试安排表size失败 in getClassSchedule()") )
            if( m_pQuery->next() && m_pQuery->value(0).toInt()==0 ) {
                m_pQuery->prepare("insert into Evaluation values(?,?,?,?,?,?,?,?,?)");
                m_pQuery->addBindValue(m_semester);
                m_pQuery->addBindValue(m_username);
                m_pQuery->addBindValue(IfSend);
                m_pQuery->addBindValue(result[2]);//被评人姓名
                if(result[3].contains("助教")) m_pQuery->addBindValue(0);
                else m_pQuery->addBindValue(1);   //被评人类型
                m_pQuery->addBindValue(result[4]);//课程名称
                m_pQuery->addBindValue(result[0]);//问卷编码
                m_pQuery->addBindValue(result[1]);//被评人系统内账号 bpr
                m_pQuery->addBindValue(result[5]);//评估内容编码 pgnr
                SQLErrorDetectForPrepare(m_pQuery, "数据插入评教表执行失败 in recordEvaluateInfo(QString &str)");
            }else {
                if( IfSend==2 )  {
                    m_pQuery->prepare("update Evaluation set IfSend=2 where Semester=? and Username=? and bpr=? and pgnr=?");
                    m_pQuery->addBindValue(m_semester);
                    m_pQuery->addBindValue(m_username);
                    m_pQuery->addBindValue(result[1]);
                    m_pQuery->addBindValue(result[5]);
                    SQLErrorDetectForPrepare(m_pQuery, "评教表数据更新失败 in recordEvaluateInfo(QString &str)");
                }
            }
    }

}
void EducationSystem::sendEvaluateInfo(QStringList userContent) {
    login();
    QEventLoop loop;QNetworkReply *pReply;
    m_request.setUrl(QUrl(m_BaseUrl+"/jxpgXsAction.do?oper=listWj&pageSize=100"));
    pReply = get(m_request);
    connect(pReply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    QSqlQuery *tmpQuery =new QSqlQuery(QSqlDatabase::database());
    SQLErrorDetect(tmpQuery, "select * from Evaluation where IfSend=1 and Username='"+m_username+"' and Semester='"+m_semester+"'", "查询用户想要发送的目标失败 in sendEvaluateInfo(QString userContent)");
    while( tmpQuery->next() ) {
        m_evaluationResult +=tmpQuery->value("CourseName").toString()+" ";
        m_lastbpr =tmpQuery->value("bpr").toString();
        m_lastpgnr =tmpQuery->value("pgnr").toString();
        QString param ="&wjbm="+tmpQuery->value("wjbm").toString()+
                "&bpr="+m_lastbpr+
                "&pgnr="+m_lastpgnr;
        m_request.setUrl(QUrl(m_BaseUrl+"/jxpgXsAction.do?oper=wjShow"+param));
        pReply =post(m_request,QByteArray());
        connect(pReply, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();
        for(int i1=0; i1<m_evaluationValue.size();++i1) {
            if( i1==userContent.size()-1 ) param.append("&"+m_evaluationValue[i1]+"=10_1");
            else param.append("&"+m_evaluationValue[i1]+"="+userContent[i1]);
        }
        param.append("&zgpj="+userContent.last());
        m_request.setUrl(QUrl(m_BaseUrl+"/jxpgXsAction.do?oper=wjpg"+param));
        post(m_request, QByteArray());
        connect(this, SIGNAL(haveGetEvaluationAfterSend()), &loop, SLOT(quit()));
        loop.exec();

    }
    emit evaluationResult(m_evaluationResult);
    m_evaluationResult ="";
    delete tmpQuery;
}
void EducationSystem::recordEvaluationValueList(QString &str) {
    m_evaluationValue =findAll(str, "<input type=\"radio\" name=\"(.*)\"", 1);
    m_evaluationValue.removeDuplicates();
}


void EducationSystem::getMaterialInfo(void) {
    login();
    m_request.setUrl(m_BaseUrl+"/zdjcAction.do?type=xsxkList");
    if( SQLErrorDetect(m_pQuery, "select count(*) from Materials where Semester='"+m_semester+"' and Username='"+m_username+"';", "探测Materials表size失败 in getMaterialInfo()") ) {
        m_pQuery->next();
        if(m_pQuery->value(0).toInt()==0 )
            get(m_request);
    }

}
void EducationSystem::recordMaterialInfo(QString &str) {
   qDebug() << "into recordMaterialInfo";
    QStringList firstResult =findAll(str, "<tr class=\"odd\" onMouseOut=\"this.className='even';\"(\\s)*onMouseOver=\"?this.className='evenfocus';\">(.*)</tr>", 2, "(&nbsp;|[\r\n\t])");
    for(int i1=0;i1<firstResult.size()-1;++i1) {
        QStringList secondResult =findAll(firstResult[i1], "<td>(.*)</td>",1, " ");
        QStringList thridResult =secondResult[9].split("<input", QString::SkipEmptyParts);
        m_pQuery->prepare("insert into Materials values(?,?,?,?,?)");
        m_pQuery->addBindValue(m_semester);
        m_pQuery->addBindValue(m_username);
        if(thridResult[0].contains("checked")) m_pQuery->addBindValue(1);
        else m_pQuery->addBindValue(0);
        m_pQuery->addBindValue(secondResult[0]);
        m_pQuery->addBindValue(findAll(thridResult[2],"value=\"(.*)\"", 1)[0]);
        SQLErrorDetectForPrepare(m_pQuery, "插入Materials表失败 in recordMaterialInfo");
    }

}
void EducationSystem::sendMaterialInfo(void) {
    SQLErrorDetect(m_pQuery, "select * from Materials", "从Materials读取数据失败 in EducationSystem::sendMaterialInfo(void)");
    QString jcStrs="";
    while( m_pQuery->next() ) {
        QString info =m_pQuery->value("Info").toString();
        bool ifChoose =m_pQuery->value("IfChoose").toBool();
        jcStrs += gbkURLCoding(info.left(info.size()-2)+QString::number(!ifChoose))+";";
    }
    login();
    m_request.setUrl(m_BaseUrl+"/zdjcAction.do?type=zdjc&jcStrs="+jcStrs);
    QString zxjxjhh=m_semester.left(9);
    if(m_semester.contains("学年春")) zxjxjhh +="-2-1";
    else zxjxjhh +="-2-2";
    QByteArray data;data.append("xh="+m_username+
                                "&zxjxjhh="+zxjxjhh);
    post(m_request,data);

}



void EducationSystem::getExamInfo(const bool force) {
    m_request.setUrl(QUrl(m_BaseUrl+"/ksApCxAction.do?oper=getKsapXx"));
    if( force==true && SQLErrorDetect(m_pQuery, "delete from Exams where Semester='"+m_semester+"' and Username='"+m_username+"';", "删除考试安排表数据失败 in getClassSchedule()") ) {
        get(m_request);
        return;
    }else {
        if( SQLErrorDetect(m_pQuery, "select count(*) from Exams where Semester='"+m_semester+"' and Username='"+m_username+"';", "探测考试安排表size失败 in getClassSchedule()") ) {
            if( m_pQuery->next() && m_pQuery->value(0).toInt()==0 )
                get(m_request);
        }
    }

}

bool EducationSystem::recordExamInfo(QString &str) {
    qDebug() << "recordExamInfo";
    QList<QString> firstResult =findAll(str, "<tr class=(\")?odd(\")?>(.*)</tr>", 3, "(&nbsp;|[ \r\n\t])");
    if( firstResult.size()==0 ) return false;
    for(auto extract : firstResult) {
        QList<QString> endResult =findAll(extract, "<td.*>(.*)</td>", 1);
        endResult.removeLast();
        endResult.removeLast();
        m_pQuery->prepare("insert into Exams values(?,?,?,?,?,?,?,?,?,?,?,?)");
        m_pQuery->addBindValue(m_semester);
        m_pQuery->addBindValue(m_username);
        for(auto value : endResult)
            m_pQuery->addBindValue(value);
        SQLErrorDetectForPrepare(m_pQuery, "数据插入成绩表执行失败 in recordExamInfo()");
    }

    return true;
}

void EducationSystem::setUnaccessible(void) {
    m_accessible =true;
    emit unaccessible();
}



//对于大包不友好
void EducationSystem::replyComplished(QNetworkReply *pReply){
    QNetworkRequest request =pReply->request();
    QString str =gbk->toUnicode(pReply->readAll());

    int size =str.size();
    qDebug() << "获得到来自"+pReply->request().url().toString()+"的正常的请求的响应";
    if( size<10000 && (str.contains("对不起、非选课阶段不允许选课")||str.contains("对不起、目前选课阶段未开放选课")) ){
        if( m_bSendClass ) {
            emit sendClassResult("非选课时间段，操作失败");
            m_bSendClass =false;
        }
    }else if( size<10000 && str.contains("<font color=\"#990000\">请您登录后再使用</font>")) {
        handleError(request);
        qDebug() << "已经完成重新登录！";
    }else if( size<10000 && str.contains("<h1>500 Servlet Exception</h1>")) {
        handleError(request);
        qDebug() << "已经完成对500 Servlet Exception的处理！";
    }else if( size<10000 && str.contains("<font color=\"#990000\">数据库忙请稍候再试</font>") ) {
        handleError(request);
        qDebug() << "已完成对数据库忙的处理";
    }else if( size<10000 && str.contains("<font color=\"#990000\">没有完成评估,不能查看课表信息!</font>") ) {
        emit needEvaluateError();
    }else  {
        QString url= pReply->request().url().toString();
        if(url.contains("/xkAction.do?actionType=")){
            QChar chActionType =url.at(45);
            switch(chActionType.digitValue()) {
                case 3:                                                                //获取公选课开课信息
                    recordSchoolPublicCourses(str);
                    emit haveGetSchoolPublicCourses();
                    break;
                case 5:
                    recordAllCourses2(str);                                              //获取本学期所有开课信息（从选课的网址）
                    break;
                case 6:                                                                 //获取本学期课表
                    if( recordClassSchedule(str) ) emit haveGetClassSchedule();
                    break;
                case 9:                                                                 //选课
                    afterSelectClass(url, str);
                    break;
                case 1:                                                                 //课程删除(对应actionType=10)
                    afterDeleteClass(url, str);
                    break;
                default:
                    ;
            }
        }else if(url.contains("/ksApCxAction.do?oper=getKsapXx")){                      //获取考试信息
            if( recordExamInfo(str) ) emit haveGetExamInfo();
        }else if(url.contains("/gradeLnAllAction.do?type=ln&oper=qbinfo")){             //获取成绩信息
            recordGrades(str);
            emit haveGetGrades();
        }else if(url.contains("/gradeLnAllAction.do?type=ln&oper=lnfaqk&flag=zx")){     //获取方案课程信息
            recordPlanCourses(str);

        }else if(url.contains("/jxpgXsAction.do?oper=listWj&pageSize=100")){            //获取评教信息
            recordEvaluateInfo(str);
            emit haveGetEvaluation();
        }else if(url.contains("/jxpgXsAction.do?oper=wjpg")) {                          //评教
            if( str.contains("评估成功")) {
                m_evaluationResult +="成功!\n";
                SQLErrorDetect(m_pQuery, "update Evaluation set IfSend=2 where Ifsend=1 and bpr='"+m_lastbpr+"' and pgnr='"+m_lastpgnr+"'","刷新评教成功数据失败 in replyComplished(QNetworkReply *pReply)");

            }else if( str.contains("alert(\"评估失败，请返回！\");") )
                m_evaluationResult+="评估失败!\n";
            emit haveGetEvaluationAfterSend();
            emit haveGetEvaluation();
        }else if(url.contains("/jxpgXsAction.do?oper=wjShow")) {                        //获取评教时应该post的数字起始值
            recordEvaluationValueList(str);
        }else if(url.contains("/courseSearchAction.do?pageNumber=")) {                  //获取本学期所有开课信息（从搜课的网址）
            recordAllCourses(str);
            emit haveGetAllCourses();
        }else if( url.contains("/zdjcAction.do?type=xsxkList") ) {
            if(size<3000) handleError(m_request);
            else { recordMaterialInfo(str);emit haveGetMaterialInfo();}

        }else if( url.contains("/zdjcAction.do?type=zdjc&jcStrs=") ) {
            if(str.contains("alert(\"选定教材成功!\");")) emit selectMaterialResult("选定教材成功！");
        }
    }
    pReply->deleteLater();
}


//处理500ServerError（除了login和logout函数） 传入error code。目前处理算是很简单。
void EducationSystem::handleError(QNetworkRequest request) {
    qDebug() << "handleError";
    login();
    QString url= request.url().toString();
    if(url.contains("/xkAction.do?actionType=3&pageNumber=")) {
        QEventLoop loop;
        m_request.setUrl(m_BaseUrl+"/xkAction.do?actionType=-1");
        QNetworkReply *pReply = get(m_request);
        connect(pReply, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();
    }else if( url.contains("/xkAction.do?actionType=9") ) {
        QRegExp rx("[=&]"); QString first =url.split(rx,QString::SkipEmptyParts)[3];
        QStringList second =first.split("_",QString::SkipEmptyParts);
        QEventLoop loop;QNetworkReply *pReply;
        m_request.setUrl(m_BaseUrl+"/xkAction.do?actionType=-1");
        pReply = get(m_request);
        connect(pReply, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();
        m_request.setUrl(m_BaseUrl+"/xkAction.do?pageNumber=-2&actionType=5&preActionType=1&kcm=&skjs=&kkxsjc=&skxq=&skjc=&&kch="+second[0]+"&cxkxh="+second[1]);
        pReply = get(m_request);
        connect(pReply, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();
    }

    //与选课发包有关的部分不能简单地重新发送，很容易陷入一直500 Servlet Error
    if(url.contains("/xkAction.do?actionType=-1") || url.contains("/xkAction.do?pageNumber=-2&actionType=5&preActionType=1&kcm=&skjs=&kkxsjc=&skxq=&skjc=&&kch=") )
        return;
    else if( url.contains("/courseSearchAction.do?pageNumber=") || url.contains("/jxpgXsAction.do?oper=wj") || url.contains("/xkAction.do?actionType=9") )
        post(request,QByteArray(""));
    else
        get(request);
}


