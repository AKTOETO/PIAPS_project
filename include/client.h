#ifndef CLIENT_H
#define CLIENT_H

#include "background.h"
#include "clientsocket.h"
#include "requestmanager.h"


// класс сервера
class Client : public Background
{
private:
    // запуск сервера
    void initClient();

public:
    Client();

protected:
    // обработка ввода с консоли
    virtual void consoleInput() override;

    // обработка логики бэкграунда
    virtual void logicProcessing() override;

    // обработка логики сокетов
    virtual void socketProcessing() override;

private:
    // сокет для запроса подключений
    ClientSocket m_socket;

    // указатель на поток выполнения обработки логики сокета
    std::unique_ptr<std::thread> m_ptr_logic;
};

#endif // !SERVER_H