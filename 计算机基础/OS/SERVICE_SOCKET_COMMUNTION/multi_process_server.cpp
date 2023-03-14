#include <asm-generic/errno.h>
#include <bits/types/sigset_t.h>
#include <cctype>
#include <cerrno>
#include <csignal>
#include <cstddef>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <ctype.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>

//信号处理函数：回收子进程
void waitchild(int signo)
{
    pid_t wpid;
    //循环是为了防止同时又多个进程退出，而信号会阻塞，所以可以多尝试退出一次
    while (1) 
    {
        wpid = waitpid(-1, NULL, WNOHANG);
        if (wpid > 0) 
        {
            std::cout << "child exit: " << wpid << '\n';
        }
        else if (wpid == 0 || wpid == -1) 
        {
            break;
        }
    }
     
}

int main()
{
    //创建监听用的文件描述符
    int lfd = socket(AF_INET, SOCK_STREAM, 0);

    //端口，ip地址的选择
    struct sockaddr_in serv;
    bzero(&serv, sizeof(serv)); //初始化结构体
    serv.sin_family = AF_INET;
    serv.sin_port = htons(8888);
    serv.sin_addr.s_addr = htonl(INADDR_ANY);

    //实现端口和监听文件描述符的绑定
    int ret = bind(lfd, (struct sockaddr *)&serv, sizeof(serv));

    //将监听文件描述符从主动转为被动
    listen(lfd, 128);

    //防止信号注册还没完成，子进程就结束了
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGCHLD); //把SIGCHLD信号加入信号集
    sigprocmask(SIG_BLOCK, &mask, NULL);//把信号集替换成阻塞信号集

    int cfd;
    pid_t pid;
    char sIP[16];
    struct sockaddr_in client;
    bzero(&client, sizeof(client));

    //建立循环的目的是为了能够反复的接收新连接和创建进程
    while (1) 
    {
        again:
        socklen_t len = sizeof(client);
        memset(&sIP, 0x00, sizeof(sIP));

        //接收新的连接，在没有可建立的链接的时候，该函数会阻塞
        cfd = accept(lfd, (struct sockaddr *)&client, &len);
        if (cfd < 0) 
        {
            //以为信号的优先级高，会打断阻塞，使函数返回-1.我们在errno中查看信号后，可以重新等待连接
            if ( (errno == EINTR) || (errno == ECONNABORTED)) goto again;
        }

        //只是为了打印一下IP地址
        auto ip = inet_ntop(AF_INET, &client.sin_addr.s_addr, sIP, sizeof(sIP));
        std::cout << "client ip = " <<  ip << " port = " << ntohs(client.sin_port) << '\n';

        //创建新的进程来处理通信
        pid = fork();
        if (pid > 0) //父进程
        {
            //父进程不需要通信，可以关闭cfd
            close(cfd);

            struct sigaction act;
            act.sa_flags = 0;
            sigemptyset(&act.sa_mask);//初始化sa_mask
            act.sa_handler = waitchild;
            //注册信号，当收到子进程结束的SIGCHLD信号，就去回收子进程
            sigaction(SIGCHLD, &act, NULL);

            //解除对SIGCHLD的阻塞
            sigprocmask(SIG_BLOCK, &mask, NULL);
        }

        else if (pid == 0) //子进程
        {
            //子进程不需要监听，关闭lfd
            close(lfd);

            char buf[1024];
            int n;
            while (1) 
            {
                //初始化缓冲区
                memset(buf, 0x00, sizeof(buf));

                //读取读缓冲区的内容，没有内容会阻塞
                n = read(cfd, &buf, sizeof(buf));
                //当read函数返回-1，说明对面关闭了通信
                if (n < 0) 
                {
                    std::cout << "client close" << '\n';
                    break;
                }

                std::cout <<"n = "<< n << "buf = "<< buf << '\n';

                //将客户端发来的信息小写转大写
                for (int i = 0; i < n; i++) 
                {
                    buf[i] = toupper(buf[i]);
                }

                //将信息发送给客户端
                write(cfd, buf, sizeof(buf));
            }
            close(cfd);
            //子进程结束后直接退出
            exit(0);
        }
    }
    close(lfd);
    return 0;
}