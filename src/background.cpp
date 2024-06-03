#include "background.h"

// запуск работы бэкграунда
void Background::Run()
{
    INFOS("Запуск бэкграунда\n");
    m_threads.emplace_back(&Background::consoleInput, this);
    m_threads.emplace_back(&Background::socketProcessing, this);
    m_threads[1].detach();
    m_threads.emplace_back(&Background::logicProcessing, this);
    m_threads[2].detach();
    m_threads[0].join();
}

void Background::addRequest(RequestSocket::Request &req)
{
    std::lock_guard<std::mutex> lock(m_request_mutex);
    m_requests.push(req);
    INFOS("Запрос добавлен\n");
    // сообщаем потоку обработки запросов, что есть необработанные запросы
    m_request_cv.notify_one();
}

Background::Background()
    : m_is_running(true)
{
}

Background::~Background()
{
}
