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
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <ctype.h>
#include <pthread.h>

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

    while (1)
    {
        tmpfds = readfds;
        again:
        nready = select(maxfd+1, &tmpfds, NULL, NULL, NULL);
        if (nready < 0) 
        {
            if (errno == EINTR || errno == ECONNABORTED) goto again;
            break;
        }

        if ( FD_ISSET(lfd, &tmpfds)) 
        {
            cfd = accept(lfd, NULL, NULL);
            FD_SET(cfd, &readfds);
            if (maxfd < cfd) 
            {
                maxfd = cfd;
            }
            
            //只有一个描述符发生变化，且是lfd时，后面不用看了
            if(--nready == 0)
            {
                continue;
            }
        }
        
        for (int i = lfd+1; i < maxfd+1; i++)
        {
            sockfd = i;
            if ( FD_ISSET(sockfd, &tmpfds)) 
            {         
                memset(&buf, 0x00, sizeof(buf));               
                n =  read(sockfd, buf, sizeof(buf));
                if ( n <= 0) 
                {
                    close(sockfd);
                    FD_CLR(sockfd, &readfds);
                    break;
                }
                else
                {
                    std::cout << n << buf << '\n';
                    for (int k = 0; k < n; k++) 
                    {
                        buf[k] = toupper(buf[k]);
                    }
                    write(sockfd, &buf, sizeof(buf));
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