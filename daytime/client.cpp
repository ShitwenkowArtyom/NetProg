#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

const int DAYTIME_PORT = 13;
const string DEFAULT_HOST = "172.16.40.1";
const int BUF_SIZE = 256;

int main(int argc, char *argv[])
{
    string host = (argc > 1) ? argv[1] : DEFAULT_HOST;

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    struct sockaddr_in srv;
    memset(&srv, 0, sizeof(srv));
    srv.sin_family = AF_INET;
    srv.sin_port = htons(DAYTIME_PORT);
    if (inet_pton(AF_INET, host.c_str(), &srv.sin_addr) != 1) {
        cerr << "Неверный адрес: " << host << endl;
        close(sock);
        return 1;
    }

    char dummy = '\0';
    sendto(sock, &dummy, 1, 0, (struct sockaddr *)&srv, sizeof(srv));

    char buf[BUF_SIZE];
    ssize_t n = recvfrom(sock, buf, sizeof(buf) - 1, 0, NULL, NULL);
    if (n < 0) {
        perror("recvfrom");
        close(sock);
        return 1;
    }
    buf[n] = '\0';

    cout << "Ответ сервера daytime (" << host << "):" << endl;
    cout << buf << endl;

    close(sock);
    return 0;
}
