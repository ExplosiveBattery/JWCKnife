#include "TitleWidget.h"


TitleWidget::TitleWidget(QWidget *parent) : QWidget(parent),m_selectedStackButtonIndex(0)
{

}


void TitleWidget::addButtons(QStringList slistComposition)
{
    int width =static_cast<QWidget*>(parent())->width()/slistComposition.size();

    m_mainLayout =new QHBoxLayout(this);
    int fontId =QFontDatabase::addApplicationFont(":/Resources/titlewidget.ttf");
    QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
    m_font.setFamily(fontFamilies.at(0));
    m_font.setPointSize(16);
    QPalette   pal;
    pal.setColor(QPalette::ButtonText, QColor(160,160,160));
    for(int i1=0; i1<slistComposition.size() ;++i1) {
        m_vButton.append(new QPushButton);
        if(i1==0) m_font.setBold(true);
        m_vButton[i1]->setFont(m_font);
        m_vButton[i1]->setPalette(pal);
        m_vButton[i1]->setText(slistComposition.at(i1));
        m_vButton[i1]->setFlat(true);
        m_vButton[i1]->setGeometry(i1*width, 0, width, 0);
        connect(m_vButton[i1], SIGNAL(clicked()), this, SLOT(handleStackButtonClick()) );
        m_mainLayout->addWidget(m_vButton[i1]);
    }
    pal.setColor(QPalette::ButtonText, QColor(0,0,0));
    m_vButton[0]->setPalette(pal);
}


void TitleWidget::handleStackButtonClick(void)
{
    m_selectedStackButtonIndex =m_vButton.indexOf(static_cast<QPushButton*>(sender()));
    QPalette   pal;
    pal.setColor(QPalette::ButtonText, QColor(160,160,160));
    for(int i1=0; i1<m_vButton.size() ;++i1) {
        if(i1==0) m_font.setBold(true);
        else m_font.setBold(false);
        m_vButton[i1]->setPalette(pal);
    }
    pal.setColor(QPalette::ButtonText, QColor(0,0,0));
    m_vButton[m_selectedStackButtonIndex]->setPalette(pal);

    //stacked widget switch
    emit stackButtonClick(m_selectedStackButtonIndex);
}

