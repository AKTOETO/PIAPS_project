#include "server.h"

void Server::initServer()
{
    INFOS("Запуск сервера\n");

    // обнуляем все множества
    FD_ZERO(&m_master_set);
    FD_ZERO(&m_read_set);

    // создаем сокет для прослушки подключений (IPv4, TCP, протокол по умолчанию)
    m_listen_socket.creates(AF_INET, SOCK_STREAM, 0);

    // выполняем привязку сокета к адресу
    m_listen_socket.binds(12321);

    // определяем длину очереди подключаемых клиентов для прослушивающего сокета
    m_listen_socket.listens(5);

    // добавляем прослушивающий сокет в мастер множество
    FD_SET(m_listen_socket(), &m_master_set);

    // устанавливаем максимальный дескриптор в множестве
    m_max_socket_desc = m_listen_socket();

    INFO("маскимальный дескриптор в множестве %d\n", m_max_socket_desc);
}

Server::Server()
    : m_listen_socket(), m_server_addres({0}), m_max_socket_desc(0)
{
    initServer();
}

void Server::Run()
{
    INFOS("Запуск работы сервера\n");
    // пока что в бесконечном цикле принимаем подключения
    // for(;;)
    // {
    //     // адрес клиента
    //     sockaddr_in client = m_server_addres;
    //     socklen_t client_len = sizeof(client);

    //     // подключаем клиента
    //     Socket client_socket;
    //     m_listen_socket.accepts((sockaddr *)&client, &client_len, &client_socket);

    //     // количество считанных байт
    //     ssize_t nread;

    //     // сообщение от клиента
    //     char buf[256];

    //     // читаем сообщение от клиента
    //     nread = read(client_socket(), buf, 256);

    //     // если же прочитать не получилось
    //     if(nread == -1)
    //     {
    //         ERRORS("Прочитать не получилось\n");
    //         throw std::runtime_error("read error");
    //     }
    //     // если пришло пустоее соощение
    //     else if(nread == 0)
    //     {
    //         INFOS("Конец файла\n");
    //     }
    //     else
    //     {
    //         // печать в консоль полученной информации от клиента
    //         write(1, buf, nread);

    //         // отправка сообщения клиенту
    //         const char *msg = "msg from server\n";
    //         write(client_socket(), msg, strlen(msg));
    //     }
    // }

    std::vector<Socket> all_clients;

    // обрабатываем сигналы с сокетов
    while (true)
    {
        // копируем мастер множество в множество четния
        m_read_set = m_master_set;

        // ждем запросы
        int res = select(m_max_socket_desc + 1, &m_read_set, NULL, NULL, NULL);

        // если результат отрицательный, произошла ошибка
        if (res == -1)
        {
            ERRORS("Ошибка при ожидании запросов на сокетах\n");
            throw std::runtime_error("select error");
        }
        else
        {
            //INFOS("Получен запрос\n");
        }
        

        // иначе проходимся по доступным дескрипторам и смотрим,
        // какой готов для чтения
        for (int i = 0; i <= m_max_socket_desc; i++)
        {
            // проверка, является ли i-ый дескриптор активным
            if (FD_ISSET(i, &m_read_set))
            {
                INFO("%d сокет активен\n", i);
                // является ли isq дескриптор - прослушивающим 
                if (i == m_listen_socket())
                {
                    // если i-ый сокет - прослушивающий сокет, значит кто-то пытается
                    // подключиться к серверу
                    all_clients.emplace_back();
                    
                    // принимаем подключение к серверу
                    sockaddr_in client_addr = m_server_addres;
                    socklen_t client_len = sizeof(client_addr);
                    m_listen_socket.accepts((sockaddr *) &client_addr, &client_len, &all_clients.back());

                    // добавляем новый сокет в мастер множество
                    FD_SET(all_clients.back()(), &m_master_set);

                    // обновляем максимальный дескрипор
                    m_max_socket_desc = std::max(m_max_socket_desc, all_clients.back()());

                    INFO("Новый клиент %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
                }
                // иначе, обрабатываем запрос на чтение с сокета
                else
                {
                    // количество прочтенных байт
                    int nread;

                    // размер буфера
                    constexpr int buf_size = 1024;

                    // буфер чтения
                    char buffer[buf_size] = {0};

                    // читаем сообщение с сокета
                    nread = read(i, buffer, buf_size);

                    // если же прочитать не получилось
                    if(nread == -1)
                    {
                        FD_CLR(i, &m_master_set);
                        ERRORS("Прочитать не получилось\n");
                        throw std::runtime_error("read error");
                    }
                    // если пришло пустоее соощение
                    else if(nread == 0)
                    {
                        FD_CLR(i, &m_master_set);
                        INFOS("Сокет закрылся со стороны клиента\n");
                    }
                    // обрабатываем полученное сообщение
                    else
                    {
                        memset(buffer + nread, 0, buf_size - nread);

                        // печать сообщения
                        printf("from client: [%s]\n ", buffer);

                        memset(buffer, '\n', buf_size);
                        // чтение с консоли
                        nread = read(STDIN_FILENO, buffer, buf_size);

                        // отправка на сервер
                        write(i, buffer, nread);
                    }
                }
            }
        }
    }
}
