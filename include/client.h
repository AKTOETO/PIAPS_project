#ifndef CLIENT_H
#define CLIENT_H

#include "includes.h"

#include "error.h"
#include "clientsocket.h"
#include "requestmanager.h"

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

    // указатель на поток выполнения обработки логики сокета
    std::unique_ptr<std::thread> m_ptr_logic;
};

#endif // !SERVER_H