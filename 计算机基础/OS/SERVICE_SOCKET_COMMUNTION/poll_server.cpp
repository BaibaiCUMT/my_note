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

int main()
{
    int lfd = socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in serv;
    serv.sin_family = AF_INET;
    serv.sin_port = htons(8888);
    serv.sin_addr.s_addr = htonl(INADDR_ANY);

    int ret = bind(lfd, (struct sockaddr *)&serv, sizeof(serv));

    listen(lfd, 128);

    int maxfd = lfd;
    fd_set readfds;
    fd_set tmpfds;

    FD_ZERO(&readfds);
    FD_ZERO(&tmpfds);
    FD_SET(lfd, &readfds);
    char buf[1024];
    int nready;
    int cfd;
    int sockfd;
    int n;
    int i;

    struct pollfd client[1024];
    for (int i = 1; i < 1024; i++) 
    {
        client[i].fd = -1;
    }

    client[0].fd = lfd;
    client[0].events = POLLIN;
    int maxi = 0;

    while (1)
    {
        tmpfds = readfds;
        again:
        //nready = select(maxfd+1, &tmpfds, NULL, NULL, NULL);
        nready = poll(client, maxi+1, -1);
        if (nready < 0) 
        {
            if (errno == EINTR || errno == ECONNABORTED) goto again;
            break;
        }

        if ( client[0].revents == POLLIN) 
        {
            cfd = accept(lfd, NULL, NULL);
            for ( i = 1; i<1024; i++) 
            {
                if (client[i].fd == -1) 
                {
                    client[i].fd = cfd;
                    client[i].events = POLLIN;
                    break;
                }
            }
            
            //只有一个描述符发生变化，且是lfd时，后面不用看了
            if(i == 1024)
            {
                close(cfd);
                continue;
            }

            if (maxi < i)
            {
                maxi = i;
            }

            if (--nready==0) 
            {
                continue;
            }
        }
        
        for (i = 1; i < maxi; i++)
        {
            if (client[i].fd == 1) 
            {
                continue;
            }

            if (client[i].revents == POLLIN) 
            {
                sockfd = client[i].fd;
                memset(&buf, 0x00, sizeof(buf));
                n =  read(sockfd, buf, sizeof(buf));

                if ( n<= 0) 
                {
                    std::cout << "client closed" << n << '\n';
                    close(sockfd);
                    client[i].fd = -1;
                }
                else 
                {
                    write(sockfd, buf, n);
                }

                if (--nready == 0) 
                {
                    break;
                }
            }
            
        }

    }
    return 0;
}