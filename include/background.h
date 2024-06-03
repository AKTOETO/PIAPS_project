#ifndef BACKGROUND_H
#define BACKGROUND_H

// реализация работы серверной части и слиентской
// три потока:
// 1 - обработка сигналов с сокета и постановка запрососв в очередь на обработку
// 2 - обработка сигналов с консоли (GUI) и постановка запрососв в очередь на обработку
// 3 - обработка запросов из очереди

#include "requestsocket.h"
#include "requestmanager.h"

class Background
{
public:

    // запуск работы бэкграунда
    virtual void Run();

    // добавить запрос
    void addRequest(RequestSocket::Request& req);

    Background();
    virtual ~Background();

protected:
    // обработка ввода с консоли
    virtual void consoleInput() = 0;

    // обработка логики бэкграунда
    virtual void logicProcessing() = 0;

    // обработка логики сокетов
    virtual void socketProcessing() = 0;

protected:
    // работает ли сервер
    std::atomic_bool m_is_running;

    // список потоков 
    std::vector<std::thread> m_threads;

    // блокировка доступа к очереди запросов с разных потоков
    std::mutex m_request_mutex;

    // для пробуждения процесса обработки запросов, когда они появляются
    std::condition_variable m_request_cv;

    // очередь запросов от сокетов
    std::queue<RequestSocket::Request> m_requests;
};

#endif // !BACKGROUND_H