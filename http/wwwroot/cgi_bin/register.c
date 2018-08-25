#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
    //username=admin&email=844493151%44qq.com&Telephone=13484514023&password=123456
    
    char* temp = strtok(buf,"&");
    strcpy(user->name,temp);
    strcpy(user->name,user->name+9);

    temp = strtok(NULL,"&");
    strcpy(user->mail,temp);
    strcpy(user->mail,user->mail+6);
     
    temp = strtok(NULL,"&");
    strcpy(user->phone,temp);
    strcpy(user->phone,user->phone+10);

    temp = strtok(NULL,"&");
    strcpy(user->passwd,temp);
    strcpy(user->passwd,user->passwd+7);

    return 0;
}
int InsertSql()
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
    sprintf(sql,"insert into user values('%s','%s','%s','%s');",user.name,user.mail,user.phone,user.passwd);
    if(mysql_query(&m_conn,sql))
    {
        perror("mysql_real_connect");
        return -1;//链接失败
    }
    //释放结果集
    mysql_close(&m_conn);
    return 0;
}
int main()
{
    printf("HTTP/1.0 200 OK\r\n");
    printf("Content-Type:text/html\r\n");
    printf("\r\n");

    if(InsertSql()==-1)
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

