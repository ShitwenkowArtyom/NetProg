#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

const int ECHO_PORT = 7;
const string DEFAULT_HOST = "172.16.40.1";
const int BUF_SIZE = 1024;

int main(int argc, char *argv[])
{
    string host = (argc > 1) ? argv[1] : DEFAULT_HOST;

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in srv;
    memset(&srv, 0, sizeof(srv));
    srv.sin_family = AF_INET;
    srv.sin_port = htons(ECHO_PORT);
    if (inet_pton(AF_INET, host.c_str(), &srv.sin_addr) != 1) {
        cerr << "Неверный адрес: " << host << endl;
        close(sock);
        return 1;
    }

    if (connect(sock, (struct sockaddr *)&srv, sizeof(srv)) < 0) {
        perror("connect");
        close(sock);
        return 1;
    }

    string msg = "Hello from NetProg echo client!\n";
    if (send(sock, msg.c_str(), msg.length(), 0) < 0) {
        perror("send");
        close(sock);
        return 1;
    }

    char buf[BUF_SIZE];
    ssize_t n = recv(sock, buf, sizeof(buf) - 1, 0);
    if (n < 0) {
        perror("recv");
        close(sock);
        return 1;
    }
    buf[n] = '\0';

    cout << "Отправлено: " << msg;
    cout << "Получено:   " << buf;

    close(sock);
    return 0;
}

