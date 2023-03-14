#include <sys/socket.h>
#include <cstdlib>
#include <cstring>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cctype>
#include <sys/un.h>
#include <iostream>

int main()
{
    int cfd = socket(AF_UNIX, SOCK_STREAM, 0);
    
    unlink("./client.sock");

    struct sockaddr_un client;
    client.sun_family = AF_UNIX;
    strcpy(client.sun_path, "./client.sock");
    int tmp = bind(cfd, (struct sockaddr *)&client, sizeof(client));

    struct sockaddr_un serv;
    serv.sun_family = AF_UNIX;
    strcpy(serv.sun_path, "./serv.sock");
    int ret = connect(cfd, (struct sockaddr *)&serv, sizeof(serv));

    char buf[1024];
    while (1)
    {
        int n = read(STDIN_FILENO, buf, sizeof(buf));

        write(cfd, buf, sizeof(buf));

        memset(&buf, 0x00, sizeof(buf));
        n = read(cfd, buf, sizeof(buf));
        std::cout << buf << '\n';
    }
    close(cfd);
}