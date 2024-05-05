#ifndef SERVER_H
#define SERVER_H

// пример клиент-серверной проги https://www.youtube.com/watch?v=09_2hrqAXQ0

#include "inculdes.h"

#include "error.h"

// класс сервера
class Server
{
private:
    // запуск сервера
    void initServer();

    // создание сокета
    int createSocket(int domain, int type, int protocol);

    // привязка сокета к адресу
    void socketBind(int sockfd, const sockaddr *addr, socklen_t addrlen);

    // задание значения очереди подключаемых клиентов
    void socketListen(int sockfd, int backlog);

    // принимаем подключение по прослушивающему сокету
    int socketAccept(int sockfd, sockaddr *addr, socklen_t *addrlen);

public:
    Server();

    // начало работы
    void Run();

private:
    // сокет для прослушивания подключений
    int m_listen_socket;

    // адрес сервера
    sockaddr_in m_server_addres;

    // список сокетов клиентов
    //std::vector<
};

#endif // !SERVER_H