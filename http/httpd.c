#include "httpd.h"

typedef struct Hander_request{
    char first_line[SIZE];
    char* method;
    char* url;
    char* url_path;
    char* query_string;
    int content_length;
}Hander_request;
int ReadLine(int new_sock,char line[],ssize_t size)
{
    //按行从sock读取数据
    //换行符：\r \n \r\n
    //1.循环地从sock中读取数据，一次读取一个
    int c='0';
    ssize_t index=0;
    while(index<size)
    {
        ssize_t r=recv(new_sock,&c,1,0);
        //读有误
        if(r<=0)
        {
            ERR_EXIT("recv");
            return -1;
        }
        //读到了\r
        if(c=='r')
        {
            //尝试从缓冲区中读取下一个，如果是\n，就处理为\n
            recv(new_sock,&c,1,MSG_PEEK);//预读，只是读上来看看，并不从缓冲区拿走
            if(c=='\n')
            {
                recv(new_sock,&c,1,0);//如果是\r\n，就将\n删掉，删掉意味着从缓冲区读上来，但是不做处理
            }
            //如果下一个不是\n，就将\r变成\n
            else
            {
                c='\n';
            }
        }
        line[index++]=c;
        if(c=='\n')
        {
            break;
        }
    }
    line[index]='\0';
    return index;
}
ssize_t Split(char* first_line,char* split,char* output[])
{   
    char* tmp=NULL;
    int index=0;
    char* p=strtok_r(first_line,split,&tmp);
    while(p!=NULL)
    {
        output[index++]=p;
        p=strtok_r(NULL,split,&tmp);
    }
    output[index]=NULL;
    return index;
}
int ParseFirstLine(char first_line[],char** url,char** method)
{
    char* toks[10]={NULL};
    ssize_t n=Split(first_line," ",toks);    
    if(n!=3)
    {
        ERR_EXIT("Split");
        return -1;
    }
    //GET url HTTP1.0
    *method=toks[0];
    *url=toks[1];
    return 0;
}
int ParseQueryString(char* url,char** url_path,char** query_string)
{
   char* p=url;
   *url_path=url;
   while(*p!='0')
   {
        if(*p=='?')
        {
            *p='\0';
            *query_string=p+1;
            return 0;
        }
        p++;
   }
   query_string=NULL;
   return 0;
}
//这个函数有两个功能
//    a 将conten_length读出来
//    b 将缓冲区内容读完，避免粘包问题
int ParseHander(int new_sock,int* content_length)
{
    char buf[SIZE]={0};
    while(1)
    {
        if(ReadLine(new_sock,buf,sizeof(buf)-1)<0)
        {
            return -1;
        }
        if(strcmp(buf,"\n")==0)
        {
            return 0;
        }
        const char* content_length_str="content_length: ";//有个空格，16个字符
        if(strncmp(buf,content_length_str,16)==0)
        {
            //将conten_length取出来了
            *content_length=atoi(buf+strlen(content_length_str));
        }
        //继续读完缓冲区内剩下地内容
    }
    return 0;
}
void HandleStatic(int new_sock,Hander_request* request)
{
    char* file_path=NULL;
    sprintf(file_path,"./wwwroot%s",request->url_path);
}
void HandlerRequest(int new_sock)
{
    Hander_request request;
    memset(&request,0,sizeof(request));
    int err_code=200;
    //1.读取请求首行
    if(ReadLine(new_sock,request.first_line,sizeof(request.first_line)-1)<0)
    {
        err_code=404;
        goto End;
    }
    printf("first_line is :%s\n",request.first_line);
    //2.解析首行，获取method，url
    if(ParseFirstLine(request.first_line,&request.url,&request.method)<0)
    {
        err_code=404;
        goto End;
    }
    //3.从url中解析出url_path,query_string
    if(ParseQueryString(request.url,&request.url_path,&request.query_string)<0)
    {
        err_code=404;
        goto End;
    }
    //4.读取并解析header，只保留content_length，其他丢弃
    if(ParseHander(new_sock,&request.content_length)<0)
    {
        err_code=404;
        goto End;
    }   

    //现在判断是GET还是POST方法
    //GET Query里有参数：执行动态页面逻辑
    //GET 无参数 ：执行静态页面逻辑
    //POST：执行动态页面逻辑
    
    if(strcasecmp(request.method,"GET")==0 && request.query_string==NULL)
    {
        //静态
        err_code=HandleStatic(new_sock,&request);
    }
    else if(strcasecmp(request.method,"GET")==0 && request.query_string!=NULL)
    {
        //动态
        err_code=HandlerCGI(new_sock,&request);
    }
    else if(strcasecmp(request.method,"POST")==0)
    {
        //动态
        err_code=HandlerCGI(new_sock,&request);
    }
    else
    {
        err_code=404;
        goto End;
    }
End:
    if(err_code!=200)
    {
        Handler404();
    }
    close(new_sock);
}
void* HandlerEntry(void* arg)
{
    int new_sock=(uint64_t)arg;
    HandlerRequest(new_sock);
    return NULL;
}
int StartUp(int port)
{
    //1.建立tcp链接
    int sock=socket(AF_INET,SOCK_STREAM,0);
    if(sock<0)
    {
        ERR_EXIT("socket");
        return 1;
    }
    int opt=1;
    setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

    //2.绑定
    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=htonl(INADDR_ANY);
    addr.sin_port=htons(port);
    if(bind(sock,(sockaddr*)&addr,sizeof(addr))==-1)
    {
        ERR_EXIT("bind");
        return 2;
    }
    if(listen(sock,5)==-1)
    {
        ERR_EXIT("listen");
        return 3;
    }
    printf("StartUp Success!\n");
    return sock;
}
void ServerStart(int port)
{
    int sock=StartUp(port);
    while(1)
    {
        struct sockaddr_in peer;
        socklen_t len=sizeof(peer);
        uint64_t new_sock=accept(sock,(struct sockaddr*)&peer,&len);
        if(new_sock<0)
        {
            ERR_EXIT("accept");
            continue;
        }
        pthread_t tid;
        pthread_create(&tid,NULL,HandlerEntry,(void*)new_sock);
        pthread_detach(tid);
    }
}
int main(int argc,char* argv[])
{
    if(argc!=2)
    {
        printf("Usage: %s [port]\n",argv[0]);
        return 1;
    }
    signal(SIGPIPE,SIG_IGN);
    ServerStart(atoi( argv[1] ));
    return 0;
}
