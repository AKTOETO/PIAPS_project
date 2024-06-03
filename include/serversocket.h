#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

#include "requestsocket.h"

class ServerSocket : public RequestSocket
{
public:

    // уникальный указатель на сокет
    using pSSocket = std::shared_ptr<ServerSocket>;

    // создание сокета
    ServerSocket(int domain, int type, int protocol);
    ServerSocket(int descriptor);
    ServerSocket();

    // удаление сокета
    ~ServerSocket();

    // привязка сокета к адресу
    void binds(const sockaddr *addr, socklen_t addrlen);
    void binds(int port, uint32_t ip = INADDR_ANY);
    void binds(int port, const char *ip);

    // задание значения очереди подключаемых клиентов
    void listens(int backlog);

    // принимаем подключение по прослушивающему сокету
    pSSocket accepts(sockaddr *addr, socklen_t *addrlen);
    pSSocket accepts();

    // функция обработки логики
    virtual void *processLogic() override;

};

#endif // !SERVERSOCKET_H