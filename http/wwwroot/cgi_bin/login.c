#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include </usr/include/mysql/mysql.h>
#define MAX_LEN 1024
//数据库插入操作
char sql[MAX_LEN];
MYSQL m_conn;
typedef struct Person
{
    char name[12];
    char mail[30];
    char phone[12];
    char passwd[12];
}Person;

int Acquire_env(Person* user)
{
    char *buf=getenv("QUERY_STRING");
    
    //username=admin&passwd=123456&submit=submit
    char* temp = strtok(buf,"&");
    strcpy(user->name,temp);
    strcpy(user->name,user->name+9);

    temp = strtok(NULL,"&");
    strcpy(user->passwd,temp);
    strcpy(user->passwd,user->passwd+7);

    return 0;
}
int SelectSql()
{
    Person user;
    bzero(&user,sizeof(user));
    Acquire_env(&user);

    mysql_init(&m_conn);
    if(!mysql_real_connect(&m_conn,"localhost","root","123456","http",3306,NULL,0))
    {
        perror("mysql_real_connect");
        return -1;//链接失败
    }

    sprintf(sql,"select * from user where name='%s' and passwd='%s';",user.name,user.passwd);
    if(mysql_query(&m_conn,sql)!=0)
    {
        return -1;
    }
    mysql_close(&m_conn);
    return 0;
}

int main()
{
    printf("HTTP/1.0 200 OK\r\n");
    printf("Content-Type:text/html\r\n");
    printf("\r\n");

    if(SelectSql()==-1)
    {
        printf("<head>");
        printf("<meta http-equiv=\"refresh\" content=\"1,url=../asserts/404.html\">");
        printf("<meta charset=\"utf-8\">");
        printf("<html>");
        printf("<body>");
        printf("</body>");
        printf("</html>");
    }
    printf("<head>");
    printf("<meta http-equiv=\"refresh\" content=\"1,url=../service.html\">");
    printf("<meta charset=\"utf-8\">");
    printf("<html>");
    printf("<body>");
    printf("</body>");
    printf("</html>");
}
