#include <cstring>
#include <netinet/in.h>
#include <strings.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <ctype.h>


int main()
{
    int cfd = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in serv;
    bzero(&serv, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(8888);
    serv.sin_addr.s_addr = htonl(INADDR_ANY);
    int ret = bind(cfd, (struct sockaddr *)&serv, sizeof(serv));

    int n;
    char buf[1024];

    struct sockaddr client;
    socklen_t len = sizeof(client);

    while (1)
    {
        memset(&buf, 0x00, sizeof(buf));
        n = recvfrom(cfd, &buf, sizeof(buf), 0, (struct sockaddr *)&client, &len);

        for (int k = 0; k < n; k++) 
        {
            buf[k] = toupper(buf[k]);
        }

        sendto(cfd, &buf, sizeof(buf), 0, (struct sockaddr *)&client, len);
    }
    close(cfd);
}