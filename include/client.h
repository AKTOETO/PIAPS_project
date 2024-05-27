#ifndef CLIENT_H
#define CLIENT_H

// пример клиент-серверной проги https://www.youtube.com/watch?v=09_2hrqAXQ0

#include "inculdes.h"

#include "error.h"
#include "clientsocket.h"

// класс сервера
class Client
{
private:
    // запуск сервера
    void initClient();

public:
    Client();

    void Run();

private:
    // сокет для запроса подключений
    ClientSocket m_socket;
};

#endif // !SERVER_H