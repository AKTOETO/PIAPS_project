#include "client.h"
void Client::initClient()
{
    INFOS("Запуск клиента\n");

    // создали сокет
    m_socket.creates(AF_INET, SOCK_STREAM, 0);

    // отправляем запрос серверу
    m_socket.connects("127.0.0.1", 12321);

    // добавление коллбэка для сокета
    m_socket.setCallback(std::bind(&Client::addRequest, this, std::placeholders::_1));
}

Client::Client()
{
    initClient();
}

void Client::consoleInput()
{
    std::string str;
    while (m_is_running && std::getline(std::cin, str))
    {
        printf("console> [%s]\n", str.c_str());

        // если пришла команда exit - выходим
        if (str == "exit")
        {
            INFOS("Выходим\n");
            m_is_running = false;
            // завершение обработки запросов
            // m_request_cv.notify_all();
            return;
        }
    }
}

void Client::logicProcessing()
{
    while(m_is_running)
    {
        // setData

        


        std::unique_lock<std::mutex> lock(m_request_mutex);
        // ждем, пока не появятся запросы
        m_request_cv.wait(lock);

        // если еще работаем
        if (m_is_running)
        {
            INFOS("Обрабатываем запрос\n");

            // выбираем запрос из очереди
            auto req = m_requests.front();
            m_requests.pop();

            // если пришел пустой запрос, значит сокет закрыдся
            if(req.m_data == "")
            {}
            // иначе, обрабатываем запрос с сокета
            else
            {
                req.m_socket->setData(RequestManager::getResponse(req.m_data));               
            }
        }
    }
}

void Client::socketProcessing()
{
    m_socket.processLogic();
}
