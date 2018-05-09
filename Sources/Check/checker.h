#ifndef CHECKER_H
#define CHECKER_H

#include <QNetworkInterface>

extern char g_keyBuf[];

class Checker
{
private:

public:
   static bool check(void);
private:
    static void m_Write(void);
    static void m_Read(void);
    static int m_GetKey(void);
};

#endif // CHECKER_H
