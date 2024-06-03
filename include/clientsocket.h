#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include "requestsocket.h"
#include "requestmanager.h"

class ClientSocket : public RequestSocket
{
public:
    // уникальный указатель на сокет
    using pCSocket = std::shared_ptr<ClientSocket>;

    // создание сокета
    ClientSocket(int domain, int type, int protocol);
    ClientSocket(int descriptor);
    ClientSocket();

    ~ClientSocket();

    // отправляем запрос на подключение
    void connects(const sockaddr *addr, socklen_t addrlen);
    void connects(uint32_t ip, int port);
    void connects(const char *ip, int port);

    // функция обработки логики
    virtual void *processLogic() override;
};

#endif // !CLIENTSOCKET_H