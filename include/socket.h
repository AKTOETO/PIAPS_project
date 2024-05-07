#ifndef SOCKET_H
#define SOCKET_H

#include "inculdes.h"
#include "error.h"

class Socket
{
public:
    // создание сокета
    Socket(int domain, int type, int protocol);
    Socket() {};

    void creates(int domain, int type, int protocol);

    // удаление сокета
    ~Socket();

    // привязка сокета к адресу
    void binds(const sockaddr *addr, socklen_t addrlen);
    void binds(int port, uint32_t ip = INADDR_ANY);

    // задание значения очереди подключаемых клиентов
    void listens(int backlog);

    // принимаем подключение по прослушивающему сокету
    void accepts(sockaddr *addr, socklen_t *addrlen, Socket *new_socket);

    // получение дескриптора сокета
    int operator()();

private:
    int m_socket; // дескриптор сокета
};

#endif // !SOCKET_H