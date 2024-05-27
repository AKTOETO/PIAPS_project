#ifndef SERVER_H
#define SERVER_H

// пример клиент-серверной проги https://www.youtube.com/watch?v=09_2hrqAXQ0

#include "inculdes.h"

#include "error.h"
#include "serversocket.h"

// класс сервера
class Server
{
private:
    // запуск сервера
    void initServer();

public:
    Server();
    ~Server();

    // начало работы
    void Run();

    // обработка входящихй соединений
    void connectionProcessing();

    // обработка ввода с консоли
    void consoleInput();

    // удаление ненужных сокетов
    void deleteSocketProcessing();

private:
    // работает ли сервер
    std::atomic_bool m_is_running;

    // адрес сервера
    sockaddr_in m_server_addres;

    // сокет сервера
    ServerSocket::pSSocket m_listen_socket;

    // список потоков-обработчиков каждого клиента
    std::vector<std::thread> m_clients_threads;

    // список сокетов клиентов
    std::vector<ServerSocket::pSSocket> m_clients_sockets;

    // блокировка доступа к спсикам потоков и клиентов
    std::mutex m_mutex;

    //// множества сокетов
    //fd_set m_master;    // множество сокетов
    //SocketSet m_master_set;    // главное множество
    //SocketSet m_read_set;      // множество чтения

    // максимальный дескриптор сокета
    //int m_max_socket_desc;
};

#endif // !SERVER_H