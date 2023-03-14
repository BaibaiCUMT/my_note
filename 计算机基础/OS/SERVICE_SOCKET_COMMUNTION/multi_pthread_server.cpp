#include <asm-generic/errno-base.h>
#include <asm-generic/errno.h>
#include <cctype>
#include <cerrno>
#include <cstddef>
#include <pthread.h>
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
#include <pthread.h>

//typedef 是将一个东西起一个名，这里是info 称作 INFO
typedef struct info
{
    int cfd;
    int idx;
    pthread_t thread;
    struct sockaddr_in client;
} INFO;

INFO thInfo[1024];

void *thread_work(void *arg)
{
    INFO *p = (INFO *)arg;
    std::cout << "idx == " << p->idx << '\n';

    char sIP[16];
    memset(sIP, 0x00, sizeof(sIP));
    std::cout << "new client: " << inet_ntop(AF_INET, &(p->client.sin_addr.s_addr), sIP, sizeof(sIP));

    int n;
    int cfd = p->cfd;
    struct sockaddr_in client;
    memcpy(&client, &(p->client), sizeof(client));

    char buf[1024];

    while (1)
    {
        memset(&buf, 0x00, sizeof(buf));
        n = read(cfd, buf, sizeof(buf));
        if (n <= 0)
        {
            std::cout << "client close: " << n <<'\n';
            close(cfd);
            p->cfd = -1;
            break;
        }

        for (int i = 0; i < n; i++)
        {
            buf[i] = toupper(buf[i]);
        }

        write(cfd, buf, sizeof(buf));
    }
    close(cfd);
    pthread_exit(NULL);
}

void init_thInfo()
{
    int i = 0;
    for (i = 0; i < 1024; i++)
    {
        thInfo[i].cfd = -1;
    }
}

int findIndex()
{
    int i;
    for (i = 0; i < 1024; i++)
    {
        if (thInfo[i].cfd == -1)
        {
            break;
        }
    }
    if (i == 1024)
    {
        return -1;
    }
    return i;
}

int main()
{
    int lfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv;
    bzero(&serv, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(8888);
    serv.sin_addr.s_addr = htonl(INADDR_ANY);
    socklen_t s_len = sizeof(serv);

    int ret = bind(lfd, (struct sockaddr *)&serv, s_len);

    listen(lfd, 128);

    init_thInfo();

    struct sockaddr_in client;
    socklen_t len;
    char sip[16];
    pthread_t thread;
    int idx;

    while (1)
    {
        len = sizeof(client);
        bzero(&client, sizeof(client));
        again:
        int cfd = accept(lfd, (struct sockaddr *)&client, &len);
        if (cfd < 0)
        {
            if (errno == EINTR || errno == ECONNABORTED) goto again;
        }

        idx = findIndex();
        if (idx == -1)
        {
            close(cfd);
            continue;
        }

        thInfo[idx].cfd = cfd;
        thInfo[idx].idx = idx;
        memcpy(&thInfo[idx].client, &client, sizeof(client));

        std::cout << "client ip: " << inet_ntop(AF_INET, &client.sin_addr.s_addr, sip, sizeof(sip)) << " port: " << ntohs(client.sin_port) << '\n';

        pthread_create(&thInfo[idx].thread, NULL, thread_work, &thInfo[idx]);
        pthread_detach(thInfo[idx].thread);

    }
    close(lfd);
    return 0;
}