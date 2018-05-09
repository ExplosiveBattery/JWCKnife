#include <QDebug>
#include <QTime>
//SQL
#include <QSqlQuery>
#include <QSqlError>
#include <QRegExp> //findAll
//UI
#include <QCoreApplication> //exitz
#include <QTableWidget> //fillTableWidget
//decode encode
#include <QTextCodec>
char g_keyBuf[]="abcd"; //预留存放硬件信息的控件
QTextCodec *gbk = QTextCodec::codecForName("GB18030");

//为了防止被ban，除了响应鼠标事件意外的所有事情都进行停止的“休眠”
void sleep(int ms) {
    QTime t;
    t.start();
    while(t.elapsed()<ms)
        QCoreApplication::processEvents();
}

//执行数据库命令
bool SQLErrorDetect (QSqlQuery *pQuery, const QString strCommand,const char *info) {
    if (!pQuery->exec(strCommand)){
        QSqlError lastError =pQuery->lastError();
        qDebug() << info  << " : "<< lastError.driverText();
        return false;
    }
     return true;
}
//执行数据库命令
bool SQLErrorDetectForPrepare (QSqlQuery *pQuery,const char *info) {
    if (!pQuery->exec()){
        QSqlError lastError =pQuery->lastError();
        qDebug() << info << " : " << lastError.driverText();
        return false;
    }
    pQuery->clear();
    return true;
}



//作用：匹配返回正则表达式中第capPos个(...)，同时第四个惨呼指定了过滤
QList<QString> findAll(const QString text, const QString regexp, int capPos, const QString removeRules) {
    QList<QString> resultSet;
    QRegExp rx(regexp);rx.setMinimal(true);
    QRegExp rxRemove(removeRules);rxRemove.setMinimal(true);
    int pos = 0;
    while ((pos = rx.indexIn(text, pos)) != -1){
        pos += rx.matchedLength();
        QString result =(removeRules=="")? rx.cap(capPos) : rx.cap(capPos).remove(rxRemove);
        resultSet << result;
    }
    return resultSet;
}

/*
 * setHorizontalHeaderLabels(header); 要在本函数之后使用
 */
void fillTableWidget(QTableWidget *tableWidget, QSqlQuery *pQuery, const QString strCommand,const char *info) {
    SQLErrorDetect(pQuery, strCommand, info);

    //先通过加入一行数据来确定columnCount
    pQuery->next();
    int columnCount=0;
    for(columnCount=0; pQuery->value(columnCount).isValid() ;++columnCount ) {
        tableWidget->setColumnCount(1+columnCount);
        tableWidget->setItem(0, columnCount, new QTableWidgetItem( pQuery->value(columnCount).toString()));
    }
    tableWidget->setRowCount(1);
    //插入剩余数据，顺便设置行数
    for(int row =1; pQuery->next() ; ++row) {
        tableWidget->setRowCount(1+row);
        for(int column=0; column<columnCount; ++column)
            tableWidget->setItem(row, column, new QTableWidgetItem( pQuery->value(column).toString()));
    }
}

 QString gbkURLCoding(QString str)
 {
     char hex[17]="0123456789ABCDEF";
     QString result ="";
     QByteArray data =gbk->fromUnicode(str);
     data =data.toPercentEncoding();
     return data;
 }
