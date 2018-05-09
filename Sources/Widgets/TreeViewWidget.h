#ifndef TREEVIEWWIDGET_H
#define TREEVIEWWIDGET_H

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QCloseEvent>

namespace Ui {
class TreeViewWidget;
}

class TreeViewWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TreeViewWidget(QWidget *parent = 0);
    ~TreeViewWidget();
    void setModel(QAbstractItemModel *model);
    void expendsAll(void);
signals:
    void close();
private:
    void closeEvent(QCloseEvent *event);

private:
    Ui::TreeViewWidget *ui;
};

#endif // TREEVIEWWIDGET_H
