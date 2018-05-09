#include "ProfessionChooseForm.h"
#include "ui_ProfessionChooseForm.h"

ProfessionChooseForm::ProfessionChooseForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProfessionChooseForm)
{
    ui->setupUi(this);
    //this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    ui->label_0->setPixmap(QPixmap(":Resources/MainWindow/banner_special.png"));
    ui->label_1->setPixmap(QPixmap(":Resources/MainWindow/banner_computer.png"));
    ui->label_2->setPixmap(QPixmap(":Resources/MainWindow/banner_chemistry.png"));
    ui->label_3->setPixmap(QPixmap(":Resources/MainWindow/banner_medicine.png"));
    ui->label_4->setPixmap(QPixmap(":Resources/MainWindow/banner_biology.png"));
    ui->label_5->setPixmap(QPixmap(":Resources/MainWindow/banner_else.png"));
    QString baseName ="pushButton_";
    for(int i1=0; i1<53 ; ++i1) {
        connect(this->findChild<QPushButton *>(baseName+QString::number(i1)),SIGNAL(clicked()), this, SLOT(handlePushButtonClick()));
    }

}

ProfessionChooseForm::~ProfessionChooseForm()
{
    delete ui;
}


void ProfessionChooseForm::handlePushButtonClick(void) {
    this->hide();
    emit choose(static_cast<QPushButton *>(sender())->text());
}

void ProfessionChooseForm::on_pushButton_clear_clicked()
{
    this->hide();
    emit choose("");
}
