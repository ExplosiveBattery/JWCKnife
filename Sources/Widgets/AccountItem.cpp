#include "AccountItem.h"
#include <QHBoxLayout>

AccountItem::AccountItem(QWidget *parent) : QWidget(parent),m_username(new QLabel),m_delete(new QPushButton)
{
    QHBoxLayout *main_layout =new QHBoxLayout();
    main_layout->addWidget(m_username);
    main_layout->addStretch();
    main_layout->addWidget(m_delete);
    this->setLayout(main_layout);
    connect( m_delete, SIGNAL(clicked()), this, SLOT(on_pushButton_delete_clicked()) );
}

void AccountItem::setText(const QString text) {
    m_username->setText(text);
}

QString AccountItem::text(void) {
    return m_username->text();
}


void AccountItem::on_pushButton_delete_clicked(void) {
    emit deleteAccount(m_username->text());
}

void AccountItem::mouseReleaseEvent(QMouseEvent *) {
    emit showAccount(m_username->text());
}
