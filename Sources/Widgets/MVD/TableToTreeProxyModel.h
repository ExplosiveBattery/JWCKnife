#ifndef TABLETOTREEPROXYMODEL_H
#define TABLETOTREEPROXYMODEL_H
//将TableModel 转变为 TreeModel
#include <QAbstractProxyModel>
#include <QVector>
#include <QList>
#include <QSqlTableModel>
#include <QSqlQuery>


bool SQLErrorDetect (QSqlQuery *pQuery, const QString strCommand, const char *info);

class TableToTreeProxyModel : public QAbstractProxyModel
{



public:
    TableToTreeProxyModel(QObject *parent = 0);
    ~TableToTreeProxyModel();
    bool init(QString);
    //QAbstractProxyModel 虚函数
    QModelIndex mapToSource(const QModelIndex &proxy_index) const;
    QModelIndex mapFromSource(const QModelIndex &source_index) const;
    //QAbstractItemModel 虚函数
    QModelIndex index(int row,int column,const QModelIndex &proxy_index=QModelIndex()) const;
    QModelIndex parent(const QModelIndex & proxy_child) const;
    int rowCount(const QModelIndex &proxy_parent) const;
    int columnCount(const QModelIndex &proxy_parent) const;
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;\
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    //如果项目数量非常高，不能一次性加载它们，则只能在视图中需要时成批添加项目。 实现canFetchMore、fetchMore


    void clear();
public:
    QSqlQuery *m_sqlQuery;
    // 目标TreeView只有两层，通过Vector记录第一层自定义的节点。
    QVector<QModelIndex> m_vIndex;
    //与上面这个QVector对应的data,rowCount
    QVector<QString> m_vData;
    QVector<int> m_vRowCount;

};

#endif // TABLETOTREEPROXYMODEL_H
