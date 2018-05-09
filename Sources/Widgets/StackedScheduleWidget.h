#ifndef STACKEDSCHEDULEWIDGET_H
#define STACKEDSCHEDULEWIDGET_H

#include <QWidget>
#include <QMenu>
#include <QAction>
#include <QModelIndex>
#include <QFontDatabase>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTime> //随机数
#include <QTimer> //计时器
#include <QMessageBox>

bool SQLErrorDetect (QSqlQuery *pQuery, const QString strCommand, const char *info);

namespace Ui {
class StackedScheduleWidget;
}

class StackedScheduleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StackedScheduleWidget(QWidget *parent = 0);
    ~StackedScheduleWidget();
    void setSemester(QString username);
    void setUsername(QString username);
    void fillTablewidget();
private:
    void fillScheduleTablewidget(int);
signals:
    void addCourse(int, int);
    void getClassSchedule(bool);
    void sendClassInfo();
public slots:
    void showMenu(const QPoint pos);
    void handleAction(void);
    void handleHaveAddedCourse(int, int, QString);
    void flushClassSchedule(void);
    void handleNeedEvaluateError(void);
private slots:
    void on_pushButton_flush_clicked();
    void setUnaccessible();
    void on_pushButton_back_clicked();
    void on_pushButton_send_clicked();

private:
    QFont m_font;
    Ui::StackedScheduleWidget *ui;
    QSqlQuery *m_pQuery;
    QModelIndex m_menuIndex;
    QVector<QColor> m_vColor ={QColor(255,159,160),QColor(255,195,97),QColor(255,224,99),QColor(252,166,237),QColor(248,173,204),QColor(250,161,181),QColor(214,174,245),QColor(129,206,248),QColor(228,164,243),QColor(129,206,248),QColor(172,190,254),QColor(195,182,241),
                               QColor(64,224,208),QColor(255,105,180),QColor(65,105,225),QColor(245,222,179),QColor(240,128,128),QColor(214,181,188),QColor(240,214,189),QColor(255,230,215),QColor(222,230,206),QColor(222,230,206),QColor(222,230,206),QColor(247,247,197),QColor(214,230,181),QColor(115,189,140),QColor(190,221,223),QColor(222,188,140),QColor(230,165,189),QColor(247,173,174),QColor(230,188,189)};
    QColor m_specialColor =QColor(234,239,245);//使用这种颜色表示是临时加入课程表，还没有真正选中
    int m_flush; //判断是程序启动后用户刷新，还是启动时自动的刷新
    QTimer *m_timer;
    QString m_semester,m_username;
};

#endif // STACKEDSCHEDULEWIDGET_H
