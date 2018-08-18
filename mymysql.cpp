#include "mymysql.h"
#include <QMessageBox>

mymysql::mymysql()
{
    mysql_init(&mysql);
    connection = NULL;
    memset(buf,0,sizeof(buf));
}

int mymysql::sql_connect(const char *host, const char *user, const char *passwd, const char *db)
{
    connection = mysql_real_connect(&mysql,host,user,passwd,db,0,0,0);
    if(connection == NULL)
    {
        strcpy(buf,mysql_error(&mysql));
        return -1;
    }
    int state = mysql_query(connection, "set names utf8");
    if(state != 0)
    {
        memset(buf,0,sizeof(buf));
        strcpy(buf,mysql_error(&mysql));
        return -1;
    }
        return 0;  
}

int mymysql::sql_exec(const char *SQL)
{
    int state = mysql_query(connection, SQL);
    if(state != 0)
    {
        memset(buf,0,sizeof(buf));
        strcpy(buf,mysql_error(&mysql));
        return -1;
    }
    return 0;
}

int mymysql::sql_open(const char *SQL,QStandardItemModel **p)
{
    int state = mysql_query(connection, SQL);
    if(state != 0)
    {
        memset(buf,0,sizeof(buf));
        strcpy(buf,mysql_error(&mysql));
        return -1;
    }
    MYSQL_RES *result =mysql_store_result(connection);
    if(result == NULL) //没有查询结果
    {
        memset(buf,0,sizeof(buf));
        strcpy(buf,mysql_error(&mysql));
        return -1;
    }
    int rowcount=mysql_affected_rows(connection);//函数返回SQL语句执行后有多少行
    int fieldcount=mysql_field_count(connection);//函数返回SQL语句执行后有多少列
    *p=new QStandardItemModel(rowcount,fieldcount); //根据SQL语句返回的行列总数，动态的建一个model出来
    MYSQL_FIELD *field;
    int i, j;
    for(i = 0; i < fieldcount;i++)
    {
        field = mysql_fetch_field(result);
        (*p)->setHeaderData(i,Qt::Horizontal, field->name);
    }
    MYSQL_ROW row;
    for(i=0;i<rowcount;i++)//循环遍历每一行
    {
        row = mysql_fetch_row(result);
        for(j=0;j<fieldcount;j++)//循环遍历每一行当中的所有列
        {
            (*p)->setData((*p)->index(i,j,QModelIndex()),row[j]);
        }
    }
    mysql_free_result(result);
    return 0;
}

void mymysql::sql_disconnect()
{
    if(connection)
    {
        mysql_close(connection);
        connection = NULL;
    }
}

const char *mymysql::geterror()
{
    return buf;
}
