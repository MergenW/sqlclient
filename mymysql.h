#ifndef MYMYSQL_H
#define MYMYSQL_H

#include <windows.h>
//路径中使用/符号
#include <D:/mysql/include/mysql.h>
#include <string.h>
#include <QStandardItemModel>

//#include <mysql/mysql.h>

//建这个类的目的是实现封装，封装所有和mysql相关函数
class mymysql
{
public:
    mymysql();
    //第一个参数代表server IP地址，第二个参数代表用户名，第三个密码，第四个是使用数据库
    int sql_connect(const char *host, const char *user, const char *passwd, const char *db);
    int sql_exec(const char *SQL);
    int sql_open(const char *SQL,QStandardItemModel **p);
    void sql_disconnect();
    const char *geterror();
private:
    MYSQL mysql;
    MYSQL *connection;
    char buf[1024];

};

#endif // MYMYSQL_H
