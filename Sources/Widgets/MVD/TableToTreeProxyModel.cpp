#include "TableToTreeProxyModel.h"


//现在的问题就是第一层的内容不会被加上第二层的内容
TableToTreeProxyModel::TableToTreeProxyModel(QObject *parent)
    : QAbstractProxyModel(parent)
{


}

TableToTreeProxyModel::~TableToTreeProxyModel() {
    delete m_sqlQuery;
}

bool TableToTreeProxyModel::init(QString username) {
    m_sqlQuery =new QSqlQuery(static_cast<QSqlTableModel*>(sourceModel())->database());
    QString filter = static_cast<QSqlTableModel*>(sourceModel())->filter();

    SQLErrorDetect(m_sqlQuery, "select Attribute from PlanCourses where " +filter+" group by Attribute;", "获取方案课程属性失败 in TableToTreeProxyModel::TableToTreeProxyModel(QObject *parent)");
    while( m_sqlQuery->next() )
        m_vData.append(m_sqlQuery->value(0).toString());
    int i1=0;
    for( auto data : m_vData ) {
        SQLErrorDetect(m_sqlQuery, "select count(*) from PlanCourses where Attribute='"+data+"' and Username='"+username+"';", "获取方案课程指定属性的记录数量失败 in TableToTreeProxyModel::TableToTreeProxyModel(QObject *parent)");
        m_sqlQuery->next();
        m_vRowCount.append(m_sqlQuery->value(0).toInt());
        m_vIndex.append(createIndex(i1++,0));

    }
    return !m_vData.isEmpty();
}


//QAbstractProxyModel 虚函数
//下面这个函数应该返回sourceModel中的ModelINdex
QModelIndex TableToTreeProxyModel::mapToSource(const QModelIndex &proxy_index) const {
    if (!proxy_index.isValid()) return QModelIndex();
    if( m_vIndex.contains(proxy_index) ) {
        int index =proxy_index.row(), row=0;
        for(int i1=0; i1<index ; ++i1) row+=m_vRowCount[i1];
        return sourceModel()->index(row,1);
    }else {
        int index = m_vIndex.indexOf(*static_cast<QModelIndex*>(proxy_index.internalPointer()));
        int row =proxy_index.row();
        for(int i1=0; i1<index ; ++i1) row+=m_vRowCount[i1];
        return sourceModel()->index(row, proxy_index.column()+2);
    }
}
QModelIndex TableToTreeProxyModel::mapFromSource(const QModelIndex &source_index) const {
    if (!source_index.isValid())  return QModelIndex();
    int index =m_vData.indexOf(sourceModel()->data(source_index.sibling(source_index.row(),1)).toString());

    if( source_index.column()==0 ) {
        return QModelIndex();
    }else if( source_index.column()==1 ) {
        return createIndex(index,0);
    }else {
        int row =source_index.row();
        for(int i1=0; i1<index ; ++i1) row-=m_vRowCount[i1];
        return createIndex(row, source_index.column()-2, const_cast<QModelIndex*>(&m_vIndex[index]));
    }
}

//QAbstractItemModel 虚函数
/*
 * parent是一个临时变量，所以要根据parent找出index
 */
QModelIndex TableToTreeProxyModel::index(int row,int column,const QModelIndex &parent ) const {
    if( !parent.isValid() )  {
        if(row<m_vIndex.size()&&column==0) return m_vIndex[row];
        else return QModelIndex();
    }
    int index =m_vIndex.indexOf(parent);
    return createIndex(row, column, const_cast<QModelIndex*>(&m_vIndex[index]));
}
QModelIndex TableToTreeProxyModel::parent(const QModelIndex &proxy_child) const{
    if( proxy_child.internalId()==0 ) return QModelIndex();
    else return *static_cast<QModelIndex*>(proxy_child.internalPointer());
}
int TableToTreeProxyModel::rowCount(const QModelIndex &proxy_parent) const{
    if( !proxy_parent.isValid() ) return m_vData.size();
    else  return m_vRowCount[m_vIndex.indexOf(proxy_parent)];
}
/* columnCount只能是一个数
 * 子项与父项是一个column
 * if( !proxy_parent.isValid() ) return 1;
 *   else return 4;
 * 上述代码就相当于仅仅return 1;
 */
int TableToTreeProxyModel::columnCount(const QModelIndex &proxy_parent) const{

    return 4;
}
bool TableToTreeProxyModel::hasChildren(const QModelIndex &parent) const {
    return m_vIndex.contains(parent)||!parent.isValid();
}
Qt::ItemFlags TableToTreeProxyModel::flags(const QModelIndex &index) const {
    if(m_vIndex.contains(index)||!index.isValid()) return Qt::ItemIsSelectable|Qt::ItemIsEnabled;
    else return Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemNeverHasChildren;
}
QVariant TableToTreeProxyModel::data(const QModelIndex &proxy_index, int role) const{
    if(proxy_index.column()==0 && proxy_index.parent().isValid() && role==Qt::DisplayRole)
        return sourceModel()->data(mapToSource(proxy_index),role).toString()=="1"?QVariant("[已修]"):"";
    return sourceModel()->data(mapToSource(proxy_index),role);

}
QVariant TableToTreeProxyModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        switch (section) {
        case 0:
            return "修读情况";
        case 1:
            return "课程号";
        case 2:
            return "课程名";
        case 3:
            return "学分";
        default:
            break;
        }
    return QVariant();
}

void TableToTreeProxyModel::clear() {
    m_vData.clear();
    m_vIndex.clear();
    m_vRowCount.clear();
}
