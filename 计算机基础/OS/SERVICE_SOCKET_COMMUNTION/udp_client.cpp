#include <arpa/inet.h>
#include <cstring>
#include <netinet/in.h>
#include <strings.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <ctype.h>
#include <iostream>


int main()
{
    int cfd = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in serv;
    bzero(&serv, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(8888);
    inet_pton(AF_INET, "127.0.0.1", &serv.sin_addr.s_addr);

    int n;
    char buf[1024] ;

    while (1)
    {
        memset(&buf, 0x00, sizeof(buf));
        n = read(STDIN_FILENO, &buf, sizeof(buf));

        sendto(cfd, &buf, sizeof(buf), 0, (struct sockaddr *)&serv, sizeof(serv));

        memset(&buf, 0x00, sizeof(buf));
        n = recvfrom(cfd, &buf, sizeof(buf), 0, NULL, NULL); 
        std::cout << buf <<'\n';
    }
    close(cfd);
}