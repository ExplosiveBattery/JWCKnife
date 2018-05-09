#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "MainWindow.h"
#include <QListWidget>
#include "AccountItem.h"
#include <QSqlQuery>
//picture display
//#include <QPainter>
//tray icon
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
//time
#include <QDateTime>
//Thread
#include <QThread>

bool SQLErrorDetect (QSqlQuery *pQuery, const QString strCommand, const char *info);

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QDialog
{
    Q_OBJECT

private:
    void fillUsernameComboBox(void);
    void fillSemesterComboxBox(void);
    void initSQL(void);
    void createTrayIcon(void);
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);
public:
    explicit LoginWindow(QWidget *parent = 0);
    ~LoginWindow();

private slots:
    void on_pushButton_login_clicked();
    void deleteAccount(QString username);
    void showAccount(QString username);
    void trayiconActivated(QSystemTrayIcon::ActivationReason reason);
    void showWindow(void);
private:
    Ui::LoginWindow *ui;
    bool m_drag;QPoint m_windowPosition; //实现鼠标拖动功能
    QSystemTrayIcon *m_trayIcon;
    QAction *m_normalAction;
    QAction *m_quitAction;
    QSqlQuery *m_pQuery;
    QListWidget *m_usernameListWidget;
    EducationSystem *m_pAccount;
    MainWindow *m_mainWindow;
    QThread m_thread;
};

#endif // LOGIN_H
