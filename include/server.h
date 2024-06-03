#ifndef SERVER_H
#define SERVER_H

#include "background.h"
#include "serversocket.h"
#include "requestmanager.h"

// класс сервера
class Server : public Background
{
private:
    // запуск сервера
    void initServer();

    // обработка ввода с консоли
    virtual void consoleInput() override;

    // обработка логики бэкграунда
    virtual void logicProcessing() override;

    // обработка логики сокетов
    virtual void socketProcessing() override;

    // удаление ненужных сокетов
    void deleteSocketProcessing();

public:
    Server();
    ~Server();

    // // добавить запрос
    // void addRequest(ServerSocket::Request& req);

    // получить размер очереди
    size_t getReqQuSize() const;

private:
    // адрес сервера
    sockaddr_in m_server_addres;

    // сокет сервера
    ServerSocket::pSSocket m_listen_socket;

    // список потоков-обработчиков каждого клиента
    std::vector<std::thread> m_clients_threads;

    // список сокетов клиентов
    std::vector<ServerSocket::pSSocket> m_clients_sockets;

    // блокировка доступа к спискам потоков и клиентов
    std::mutex m_clients_mutex;

    // очередь запросов от сокетов
    std::queue<ServerSocket::Request> m_requests;

    // блокировка доступа к очереди запросов с разных потоков
    std::mutex m_request_mutex;

    // для пробуждения процесса обработки запросов, когда они появляются
    std::condition_variable m_request_cv;
};

#endif // !SERVER_H