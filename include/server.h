#ifndef SERVER_H
#define SERVER_H

// пример клиент-серверной проги https://www.youtube.com/watch?v=09_2hrqAXQ0

#include "inculdes.h"

#include "error.h"
#include "socket.h"

// класс сервера
class Server
{
private:
    // запуск сервера
    void initServer();

public:
    Server();

    // начало работы
    void Run();

private:
    // сокет для прослушивания подключений
    Socket m_listen_socket;

    // адрес сервера
    sockaddr_in m_server_addres;

    // множества сокетов
    fd_set m_master_set;    // главное множество
    fd_set m_read_set;      // множество чтения

    // максимальный дескриптор сокета
    int m_max_socket_desc;
};

#endif // !SERVER_H