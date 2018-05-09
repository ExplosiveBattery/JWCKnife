#include "CardArrayWidget.h"
#include <QDebug>

CardArrayWidget::CardArrayWidget(QWidget *parent) : QWidget(parent)
{

    //rand
    //qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    //font type
    int fontId =QFontDatabase::addApplicationFont(":/Resources/titlewidget.ttf");
    QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
    m_font.setFamily(fontFamilies.at(0));
}


void CardArrayWidget::addButtons(QVector<int> vSplit, QStringList slContent) {
    int width,height =this->height()/vSplit.size()*0.83;
    int posOfList=0;
    m_mainLayout =new QVBoxLayout(this);
    for(auto iButton : vSplit) {
        QHBoxLayout *hBoxLayout =new QHBoxLayout;
        for(int i1=0; i1<iButton; ++i1 ) {
            QPushButton *pushButton =new QPushButton(this);
            //set text
            if(posOfList>slContent.size()) pushButton->setText("缺少内容");
            else pushButton->setText(slContent[posOfList++]);
            //set font
            m_font.setPointSize(height*0.09);
            pushButton->setFont(m_font);
            //set other QSS
            width=this->width()/iButton*0.55;
            QString qss="max-height:"+QString::number(height)+";"
                        "max-width:"+QString::number(width)+";"
                        "border: 2px solid;";
            pushButton->setStyleSheet(qss);

            //other set
            connect(pushButton,SIGNAL(clicked()), this, SLOT(handleButtonClick()));
            hBoxLayout->addWidget(pushButton);

            m_vButton.append(pushButton);

        }
        m_mainLayout->addLayout(hBoxLayout);
    }
    setLayout(m_mainLayout);
}


void CardArrayWidget::handleButtonClick() {
    emit click(static_cast<QPushButton*>(sender())->text());
}

