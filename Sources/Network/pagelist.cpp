#include "pagelist.h"

PageList::PageList(const int b, const int e, const int add):m_iBeginBoundary(b),m_iEndBoundary(e),m_iAdd(add),pos(0)
{
    for(int i1=m_iBeginBoundary;i1 <=m_iEndBoundary;++i1 )
        m_pageList << i1;
    m_iBeginBoundary +=m_iAdd;
    m_iEndBoundary +=m_iAdd;
}

bool PageList::isEmpty(void) {
    return m_pageList.isEmpty();
}

int  PageList::getPageNumber(void){
    if(pos==m_pageList.size()){
        pos =0;
        return -1; //表示对链表的一轮完整访问已经结束
    }
    return m_pageList.at(pos++);
}

void  PageList::delPageNumber(int pos){
    if(0<=pos && pos<m_pageList.size())
        m_pageList.removeAt(pos);
}

//向列表进行范围的补充
void  PageList::supply(void){
    for(int i1=m_iBeginBoundary;i1 <=m_iEndBoundary;++i1 )
        m_pageList << i1;
    m_iBeginBoundary +=m_iAdd;
    m_iEndBoundary +=m_iAdd;
}


void  PageList::clear(void) {
    m_pageList.clear();
}
