#include "StackedPlanCoursesWidget.h"
#include <QDebug>

StackedPlanCoursesWidget::StackedPlanCoursesWidget(QWidget *parent) : QWidget(parent),m_cardArrayWidget(new CardArrayWidget(this)),m_treeView(new TreeViewWidget(nullptr)),m_model(new QSqlTableModel(this))
{
    resize(1200, 670);
    m_cardArrayWidget->resize(this->width(), this->height());
    m_model->setTable("PlanCourses");
    m_model->select();
    m_proxyModel =new TableToTreeProxyModel(this);

    connect(m_treeView, SIGNAL(close()), this, SLOT(handleWidgetClose()));
    connect(m_cardArrayWidget, SIGNAL(click(QString)), this, SLOT(showTreeWidget(QString)));
}

void StackedPlanCoursesWidget::setUsername(QString username)
{
    m_username =username;
}

void StackedPlanCoursesWidget::addButtons(QVector<int> vSplit, QStringList sContent) {
    m_cardArrayWidget->addButtons(vSplit, sContent);
}


void StackedPlanCoursesWidget::handleWidgetClose() {
    m_proxyModel ->clear();
}


void StackedPlanCoursesWidget::showTreeWidget(QString str) {
    m_model->setFilter(" Username='"+m_username+"' and Attribute like '"+str+"%'");
    m_proxyModel->setSourceModel(m_model);
    if( m_proxyModel->init(m_username)==true )  {
        m_treeView->setModel(m_proxyModel);
        m_treeView->expendsAll();
        m_treeView->show();
    }
}
