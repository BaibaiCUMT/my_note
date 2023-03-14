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
    //创建描述符
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lfd < 0) 
    {
        perror("socket error");
        return -1;
    }
    
    //创建结构体sockaddr_in
    struct sockaddr_in serv;
    bzero(&serv, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(8888);
    serv.sin_addr.s_addr = htonl(INADDR_ANY);

    int ret = bind(lfd, (struct sockaddr *)&serv, sizeof(serv));
    if (ret < 0) 
    {
        perror("bind error");
        return -1;
    }

    listen(lfd, 128);
    
//******************************
    // struct sockaddr_in client;
    // socklen_t len = sizeof(client);

    // accept(lfd, (struct sockaddr *)&client, &len);
    
    // char sIP[16];
    // printf("client ip: %s, port: %d", 
    // inet_ntop(AF_INET, &client.sin_addr.s_addr, sIP, sizeof(sIP)), 
    // ntohs(client.sin_port));
//******************************

    int cfd = accept(lfd, NULL, NULL);
    std::cout << "cfd: " << cfd  << "  lfd: " << lfd << '\n';

    int n = 0;
    char buf[1024];
    
    while(1)
    {
        memset(buf, 0x00, sizeof(buf));
        n = read(cfd, &buf, sizeof(buf));
        if (n <= 0) 
        {
            std::cout << "read error or client connect close: " << n << '\n';
            break;
        }
        std::cout << "n = " << n << "buf = " << buf <<'\n';

        for (int i = 0; i < n; i++) 
        {
            buf[i] = toupper(buf[i]);
        }
        write(cfd, buf, n);
    }
    

    close(cfd);
    close(lfd);

    return 0;
}