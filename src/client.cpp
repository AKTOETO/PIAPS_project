#include "client.h"
void Client::initClient()
{
    INFOS("Запуск клиента\n");

    // создали сокет
    m_socket.creates(AF_INET, SOCK_STREAM, 0);

    // отправляем запрос серверу
    m_socket.connects("127.0.0.1", 12321);
}

Client::Client()
{
    initClient();
}

void Client::consoleInput()
{
    std::string str;
    while(m_is_running && std::getline(std::cin, str))
    {
        printf("console> [%s]\n", str.c_str());

        // если пришла команда exit - выходим
        if (str == "exit")
        {
            INFOS("Выходим\n");
            m_is_running = false;
            // завершение обработки запросов
            //m_request_cv.notify_all();
            return;
        }
    }
}

void Client::logicProcessing()
{
}

void Client::socketProcessing()
{
    m_socket.processLogic();
}

// void Client::Run()
// {
//     INFOS("Работа клиента\n");

//     // запуск обработки логики на сокете
//     m_ptr_logic = std::make_unique<std::thread>(m_socket);

//     m_ptr_logic->join();
// }
