#include "TreeViewWidget.h"
#include "ui_TreeViewWidget.h"

TreeViewWidget::TreeViewWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TreeViewWidget)
{
    ui->setupUi(this);
}

TreeViewWidget::~TreeViewWidget()
{
    delete ui;
}


void TreeViewWidget::closeEvent(QCloseEvent *event) {
    emit close();
}


void TreeViewWidget::setModel(QAbstractItemModel *model) {
    ui->treeView->setModel(model);
}


void TreeViewWidget::expendsAll(void) {
    ui->treeView->expandAll();
}


