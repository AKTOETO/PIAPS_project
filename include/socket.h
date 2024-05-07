#ifndef SOCKET_H
#define SOCKET_H

#include "inculdes.h"
#include "error.h"

class Socket
{
private:
    // преобразрование ip адреса из чтроки в число
    void inetPton(const char *src, void *dst);

public:
    // создание сокета
    Socket(int domain, int type, int protocol);
    Socket(){};

    void creates(int domain, int type, int protocol);

    // удаление сокета
    ~Socket();

    // привязка сокета к адресу
    void binds(const sockaddr *addr, socklen_t addrlen);
    void binds(int port, uint32_t ip = INADDR_ANY);
    void binds(int port, const char *ip);

    // задание значения очереди подключаемых клиентов
    void listens(int backlog);

    // принимаем подключение по прослушивающему сокету
    void accepts(sockaddr *addr, socklen_t *addrlen, Socket *new_socket);

    // отправляем запрос на подключение
    void connects(const sockaddr *addr, socklen_t addrlen);
    void connects(uint32_t ip, int port);
    void connects(const char *ip, int port);

    // получение дескриптора сокета
    int operator()();

private:
    int m_socket; // дескриптор сокета
    int m_domain; // домен сокета (ipv4 / ipv6)
};

#endif // !SOCKET_H