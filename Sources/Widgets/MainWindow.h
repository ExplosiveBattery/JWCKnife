#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QSqlTableModel>
//sql operating
#include <QSqlDatabase>
//tray icon
#include <QSystemTrayIcon>
#include <QAction>
//Network
#include "../Network/educationsystem.h"
//move windows
#include <QMouseEvent>
//statck widget
#include <QPixmap>
#include <QPainter>
#include "StackedGradesWidget.h"
#include "StackedSelectClassWidget.h"
#include "StackedExamRelatedWidget.h"
//trayicon
#include <QMenu>
#include <QIcon>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);
public:
    explicit MainWindow(EducationSystem *pAccount,QWidget *parent = 0);
    ~MainWindow();

signals:
    void sendClassInfo();
    void sendEvaluateInfo(QStringList);

protected slots:
    void handleUnaccessible(void);
    void handleStackButtonClick(void);
    void on_pushButton_minimize_clicked(void);
    void handleEvaluationSend(QStringList userContent);
    void handleSendClass(void);
    void handleAddCourse(void);
    void informationWidget(QString);

private:
    Ui::MainWindow *ui;
    //move window
    bool m_drag;QPoint m_windowPosition;
    //network
    QSqlTableModel *m_model;
    EducationSystem *m_pAccount;
    //stack widge
    int m_selectedStackButtonX;
};

#endif // MAINWINDOW_H
