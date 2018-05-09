#include "checker.h"
#include <QFile>
#include <QMessageBox>
#include <QCryptographicHash>


//软件启动检查函数
bool Checker::check(void) {
    QFile file("debug//JWCKnife.exe");
    if(file.open(QIODevice::ReadOnly | QIODevice::Append)) {
        qDebug() << "in";
        file.seek(0x0020);//放逆向的时候对这个常量使用变量绕一圈
        //file.write((char*)&g_keyBuf);

    }
    return true;
}

//硬件绑定函数  并且经过了加密
int Checker::m_GetKey(void) {
    int ret=0;
    QNetworkInterface interface =QNetworkInterface::interfaceFromIndex(0);
    QString detail =interface.hardwareAddress();
    QByteArray bb;
    bb = QCryptographicHash::hash ( detail.toLatin1(), QCryptographicHash::Md5 );
    for(auto b : bb){
       ret+=(int)b; //这里会进行符号位的扩充，所以到很可能产生负数
    }
    return ret;
}


//从文件中读取key
void  Checker::m_Read(void) {



}

//将key写入文件
void  Checker::m_Write(void) {



}
