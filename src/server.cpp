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
    : m_is_running(true), m_server_addres({0, 0, 0, 0}) //, m_master_set(), m_read_set(), m_max_socket_desc(0)
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

void Server::Run()
{
    INFOS("Запуск работы сервера\n");

    // создание потока для обработки входящих соединений
    std::thread input_connect(&Server::connectionProcessing, this);

    // создание потока удаления ненужных сокетов
    std::thread delete_socket(&Server::deleteSocketProcessing, this);

    // создание потока обработки ввода с консоли
    std::thread input_console(&Server::consoleInput, this);

    // ожидание завершения работы вывода с консоли
    input_console.join();

    input_connect.detach();
    delete_socket.detach();
}

void Server::connectionProcessing()
{
    INFOS("Обработка соединений\n");

    // цикл обработки подключений клиентов
    while (m_is_running.load())
    {
        // вызываем accept, если нет соединений, поток блокируется
        auto new_socket = m_listen_socket->accepts();

        // критическая секция доступа к списку сокетов и потоков
        {
            std::lock_guard<std::mutex> lock(m_mutex);

            // добавляем сокет в список сокетов
            m_clients_sockets.emplace_back(new_socket);

            // создаем поток с обработкой логики сокета
            m_clients_threads.emplace_back(*new_socket);
        }
    }
}

void Server::consoleInput()
{
    INFOS("Обработка консоли\n");

    // строка считывания команды
    std::string str;

    // читаем команды в цикле
    while (m_is_running.load() && std::getline(std::cin, str))
    {
        printf("console> [%s]\n", str.c_str());

        // если пришла команда exit - выходим
        if (str == "exit")
        {
            INFOS("Выходим\n");
            m_is_running = false;
        }
    }
}

void Server::deleteSocketProcessing()
{
    while (m_is_running)
    {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            for (size_t i = 0; i < m_clients_sockets.size() && m_is_running; i++)
            {
                // если сокет помечен для удаления, удаляем его
                if (m_clients_sockets[i]->getDeleteStatus())
                {
                    m_clients_sockets.erase(m_clients_sockets.begin() + i);
                    m_clients_threads[i].join();
                    m_clients_threads.erase(m_clients_threads.begin() + i);
                }
            }
        }
    }
}
