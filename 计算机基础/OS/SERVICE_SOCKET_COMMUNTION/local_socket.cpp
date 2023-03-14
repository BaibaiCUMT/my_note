#include <sys/socket.h>
#include <cstring>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cctype>
#include <sys/un.h>

int main()
{
    int lfd = socket(AF_UNIX, SOCK_STREAM, 0);

    unlink("./serv.sock");

    struct sockaddr_un serv;
    bzero(&serv, sizeof(serv));
    serv.sun_family = AF_UNIX;
    strcpy(serv.sun_path, "./serv.sock");
    int ret = bind(lfd, (struct sockaddr *)&serv, sizeof(serv));

    listen(lfd, 128);

    struct sockaddr_un client;
    socklen_t len = sizeof(client);

    int cfd = accept(lfd, (struct sockaddr *)&client, &len);

    char buf[1024];
    while (1) 
    {
        memset(&buf, 0x00, sizeof(buf));
        int n = read(cfd, buf, sizeof(buf));

        for (int k = 0; k < n; k++)  buf[k] = toupper(buf[k]);

        write(cfd, buf, sizeof(buf));
    }
    
    close(cfd);
    close(lfd);
}