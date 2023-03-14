#include <cctype>
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

int main()
{
    int cfd = socket(AF_INET, SOCK_STREAM, 0);
    if (cfd < 0)
    {
        perror("socket error");
        return -1;
    }

    struct sockaddr_in serv;
    serv.sin_family = AF_INET;
    serv.sin_port = htons(8888);
    inet_pton(AF_INET, "127.0.0.1", &serv.sin_addr.s_addr);
    std::cout << &serv.sin_addr.s_addr <<'\n';

    int ret = connect(cfd, (struct sockaddr *)&serv, sizeof(serv));
    if (ret < 0) 
    {
        perror("connect error");
        return -1;
    }

    int n = 0;
    char buf[256];
    while (1) 
    {
        memset(buf, 0x00, sizeof(buf));
        n = read(STDIN_FILENO, &buf, sizeof(buf));

        write(cfd, buf, n);

        memset(buf, 0x00, sizeof(buf));
        n = read(cfd, buf, sizeof(buf));
        if (n <= 0) 
        {
            std::cout << "read error or service close: " << n << '\n';
            break;
        }
        std::cout << buf << '\n';
    }

    close(cfd);

    return 0;
}