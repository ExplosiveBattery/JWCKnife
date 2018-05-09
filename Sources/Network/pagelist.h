//为了配合多线程多进程进行范围的划分
#ifndef PAGELIST_H
#define PAGELIST_H

#include <QList>

class PageList
{
private:
    QList<int> m_pageList;//范围的存储列表
    int m_iBeginBoundary;//起始范围
    int m_iEndBoundary;//结束范围
    int m_iAdd;//每次范围增加的部分
    int pos;//指示已经遍历到的地方
public:
    PageList(const int b, const int e, const int add);
    bool isEmpty(void);
    int getPageNumber(void);
    void delPageNumber(int pos);
    void clear(void);
    void supply(void);
};

#endif // PAGELIST_H
