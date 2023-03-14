#include <asm-generic/errno-base.h>
#include <asm-generic/errno.h>
#include <asm-generic/socket.h>
#include <cctype>
#include <cerrno>
#include <cstddef>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <strings.h>
#include <sys/poll.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <ctype.h>
#include <pthread.h>
#include <poll.h>
#include <sys/epoll.h>

int main()
{
    int lfd = socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int));

    struct sockaddr_in serv;
    serv.sin_family = AF_INET;
    serv.sin_port = htons(8888);
    serv.sin_addr.s_addr = htonl(INADDR_ANY);
    int ret = bind(lfd, (struct sockaddr*)&serv, sizeof(serv));

    listen(lfd, 128);


    int epfd = epoll_create(1024);

    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = lfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &ev);

    struct epoll_event events[1024];
    int clie_size = 1024;

    int nready = 0;
    int cfd = 0;
    int n = 0;
    int sockfd;
    char buf[1024];

    while (1) 
    {
        memset(&buf, 0x00, sizeof(buf));
        nready = epoll_wait(epfd, events, clie_size, -1);
        if (nready < 0) 
        {
            if (errno == EINTR || errno == ECONNABORTED) 
            {
                continue;
            }
            break;
        }

        for (int i = 0; i < nready; i++) 
        {
            sockfd = events[i].data.fd;

            if (sockfd == lfd) 
            {
                cfd = accept(lfd, NULL, NULL);
                ev.data.fd = cfd;
                ev.events = EPOLLIN;
                epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev);
            }
            else
            {
                n = read(sockfd, buf, sizeof(buf));
                if (n <= 0) 
                {
                    close(sockfd);
                    epoll_ctl(epfd, EPOLL_CTL_DEL, sockfd, NULL);
                    std::cout << "client close, status: " << n << '\n';
                    continue;
                }

                for (int k = 0; k < n; k++)  buf[k] = toupper(buf[k]);

                write(sockfd, buf, sizeof(buf));
            }
        }
    }
    close(epfd);
    close(lfd);
    return 0;
}