#include "Widgets/LoginWindow.h"
#include <QApplication>
#include "Widgets/MainWindow.h"
#include "Network/educationsystem.h"

#include <QSqlQuery>
#include <QDebug>
#include <QThread>




int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(true);

    LoginWindow w;
    w.show();

//    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE");
//    db.setDatabaseName("information.db");
//    db.setUserName("sa");
//    db.setPassword("123");
//    MainWindow w(new EducationSystem("2015141462109","1234","2017-2018学年春"));
//    w.show();

    return a.exec();
}
//return code 1 means important SQL error


