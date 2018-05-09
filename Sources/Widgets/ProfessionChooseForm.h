#ifndef PROFESSIONCHOOSEFORM_H
#define PROFESSIONCHOOSEFORM_H

#include <QWidget>

namespace Ui {
class ProfessionChooseForm;
}

class ProfessionChooseForm : public QWidget
{
    Q_OBJECT

public:
    explicit ProfessionChooseForm(QWidget *parent = 0);
    ~ProfessionChooseForm();
protected slots:
    void handlePushButtonClick(void);
signals:
    void choose(QString text);

private slots:
    void on_pushButton_clear_clicked();

private:
    Ui::ProfessionChooseForm *ui;
};

#endif // PROFESSIONCHOOSEFORM_H
