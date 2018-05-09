#ifndef ACCOUNTITEM_H
#define ACCOUNTITEM_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

//loginWindow中的用户账户 下拉菜单
class AccountItem : public QWidget
{
    Q_OBJECT
private:
    QLabel *m_username;
    QPushButton *m_delete;
public:
    explicit AccountItem(QWidget *parent = 0);
    void setText(const QString text);
    QString text(void);//这里其实是getText，但是遵循了Qt里面的命名方法
    void mouseReleaseEvent(QMouseEvent *);

signals:
    void deleteAccount(QString text);
    void showAccount(QString text);
private slots:
    void on_pushButton_delete_clicked(void);

};

#endif // ACCOUNTITEM_H
