#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

#include "socket.h"

class ClientSocket : public Socket
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

#endif // !SERVERSOCKET_H