#include "server.h"

void Server::initServer()
{
    INFOS("Запуск сервера\n");

    // создаем сокет для прослушки подключений (IPv4, TCP, протокол по умолчанию)
    m_listen_socket = std::make_shared<ServerSocket>(AF_INET, SOCK_STREAM, 0);

    // заставляем слушать сокет все адреса с портом 12321
    m_listen_socket->binds(12321);

    // определяем длину очереди подключаемых клиентов для прослушивающего сокета
    m_listen_socket->listens(5);
}

Server::Server()
    : Background(), m_server_addres({0, 0, 0, 0}) //, m_master_set(), m_read_set(), m_max_socket_desc(0)
{
    initServer();
}

Server::~Server()
{
    // ожидаем завершения всех потоков
    for (auto &el : m_clients_threads)
    {
        el.join();
    }
}

// void Server::Run()
// {
//     INFOS("Запуск работы сервера\n");

//     // создание потока обработки ввода с консоли
//     std::thread input_console(&Server::consoleInput, this);

//     // создание потока для обработки входящих соединений
//     std::thread input_connect(&Server::connectionProcessing, this);

//     // создание потока обработки запросв с сокетов
//     std::thread request_process(&Server::requestProcess, this);

//     input_connect.detach();
//     request_process.detach();

//     // ожидание завершения работы вывода с консоли
//     input_console.join();
// }

// void Server::addRequest(ServerSocket::Request &req)
// {
//     // добавляем запрос от сокета в очерель на обработку
//     {
//         std::lock_guard<std::mutex> lock(m_request_mutex);
//         m_requests.push(req);
//         INFOS("Запрос добавлен\n");
//         // сообщаем потоку обработки запросов, что есть необработанные запросы
//         m_request_cv.notify_one();
//     }
// }

size_t Server::getReqQuSize() const
{
    return m_requests.size();
}

void Server::socketProcessing()
{
    INFOS("Обработка соединений\n");

    // цикл обработки подключений клиентов
    while (m_is_running)
    {
        // вызываем accept, если нет соединений, поток блокируется
        auto new_socket = m_listen_socket->accepts();
        // если получили nullptr, значит слушающий сокет закрылся
        if (new_socket == nullptr)
        {
            INFOS("Сокет закрылся\n");
            m_is_running = 0;
            break;
        }

        // установка callback функции для сокета
        new_socket->setCallback(std::bind(&Server::addRequest, this, std::placeholders::_1));

        // критическая секция доступа к списку сокетов и потоков
        {
            std::unique_lock<std::mutex> lock(m_clients_mutex);

            // добавляем сокет в список сокетов
            m_clients_sockets.emplace_back(new_socket);

            // создаем поток с обработкой логики сокета
            m_clients_threads.emplace_back(&ServerSocket::processLogic, &(*new_socket));
        }
    }
}

void Server::consoleInput()
{
    INFOS("Обработка консоли\n");

    // строка считывания команды
    std::string str;

    // читаем команды в цикле
    while (m_is_running && std::getline(std::cin, str))
    {
        printf("console> [%s]\n", str.c_str());

        // если пришла команда exit - выходим
        if (str == "exit")
        {
            INFOS("Выходим\n");
            m_is_running = false;
            // завершение обработки запросов
            m_request_cv.notify_all();
            return;
        }
    }
}

void Server::deleteSocketProcessing()
{
    std::unique_lock<std::mutex> lock(m_clients_mutex);
    for (size_t i = 0; i < m_clients_sockets.size() && m_is_running; i++)
    {
        // если сокет помечен для удаления, удаляем его
        if (m_clients_sockets[i]->getDeleteStatus())
        {
            m_clients_threads[i].detach();
            m_clients_threads.erase(m_clients_threads.begin() + i);
            m_clients_sockets.erase(m_clients_sockets.begin() + i);
            INFO("Удален %ld сокет\n", i);
            i--;
        }
    }
}

void Server::logicProcessing()
{
    while (m_is_running)
    {
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
            {
                deleteSocketProcessing();
            }
            // иначе, обрабатываем запрос с сокета
            else
            {
                req.m_socket->setData(RequestManager::getResponse(req.m_data));               
            }
        }
    }
}
