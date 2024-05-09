#include "server.h"

void Server::initServer()

{
    INFOS("Запуск сервера\n");

    // // обнуляем все множества
    // FD_ZERO(&m_master_set);
    // FD_ZERO(&m_read_set);

    // создаем сокет для прослушки подключений (IPv4, TCP, протокол по умолчанию)
    Socket::pSocket m_listen_socket(std::make_shared<Socket>(AF_INET, SOCK_STREAM, 0));

    // выполняем привязку сокета к адресу
    m_listen_socket->binds(12321);

    // определяем длину очереди подключаемых клиентов для прослушивающего сокета
    m_listen_socket->listens(5);

    // добавляем прослушивающий сокет в мастер множество
    //FD_SET(m_listen_socket->getDescr(), &m_master_set);
    m_master_set.add(m_listen_socket);

    // устанавливаем максимальный дескриптор в множестве
    //m_max_socket_desc = m_listen_socket->getDescr();

    INFO("маскимальный дескриптор в множестве %d\n", m_max_socket_desc);
}

Server::Server()
    :  m_server_addres({0,0,0,0}), m_master_set(), m_read_set(), m_max_socket_desc(0)
{
    initServer();
}

void Server::Run()
{
    INFOS("Запуск работы сервера\n");

    std::vector<Socket::pSocket> all_clients;

    // обрабатываем сигналы с сокетов
    while (true)
    {
        // копируем мастер множество в множество четния
        m_read_set = m_master_set;

        // // ждем запросы
        // int res = select(m_max_socket_desc + 1, &m_read_set, NULL, NULL, NULL);

        // // если результат отрицательный, произошла ошибка
        // if (res == -1)
        // {
        //     ERRORS("Ошибка при ожидании запросов на сокетах\n");
        //     throw std::runtime_error("select error");
        // }
        m_read_set.waitSock();

        // иначе проходимся по доступным дескрипторам и смотрим,
        // какой готов для чтения
        //for (int i = 0; i <= m_max_socket_desc; i++)
        for(auto it = m_read_set.getSockets().begin(); it != m_read_set.getSockets().end(); it++)
        {
            // проверка, является ли i-ый дескриптор активным
            //if (FD_ISSET(i, &m_read_set))
            if(m_read_set.isSocketActive(it))
            {
                INFO("%d сокет активен\n", it->first);
                // является ли iый дескриптор прослушивающим
                //if (i == m_listen_socket->getDescr())
                if(m_read_set.isSocketListening(it))
                {
                    // получаем новый сокет
                    auto ret = *((Socket::pSocket *)(*(it)->second)());

                    // добавляем его в множество
                    m_master_set.add(std::move(ret));
                    // // если i-ый сокет - прослушивающий сокет, значит кто-то пытается
                    // // подключиться к серверу
                    // all_clients.emplace_back();

                    // // принимаем подключение к серверу
                    // sockaddr_in client_addr = m_server_addres;
                    // socklen_t client_len = sizeof(client_addr);
                    // all_clients.back() = m_listen_socket->accepts((sockaddr *)&client_addr, &client_len);

                    // // добавляем новый сокет в мастер множество
                    // FD_SET(all_clients.back()->getDescr(), &m_master_set);

                    // // обновляем максимальный дескрипор
                    // m_max_socket_desc = std::max(m_max_socket_desc, all_clients.back()->getDescr());

                    // INFO("Новый клиент %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
                }
                // иначе, обрабатываем запрос на чтение с сокета
                else
                {
                    // // количество прочтенных байт
                    // int nread;

                    // // размер буфера
                    // constexpr int buf_size = 1024;

                    // // буфер чтения
                    // char buffer[buf_size] = {0};

                    // // читаем сообщение с сокета
                    // nread = read(i, buffer, buf_size);
                    // // nread =

                    // // если же прочитать не получилось
                    // if (nread == -1)
                    // {
                    //     // TODO создать структура в которой будет тип события и сокет для него
                    //     // чтобы можно было отличить удаление он добавления
                    //     FD_CLR(i, &m_master_set);
                    //     close(i);
                    //     ERRORS("Прочитать не получилось\n");
                    //     throw std::runtime_error("read error");
                    // }
                    // // если пришло пустоее соощение
                    // else if (nread == 0)
                    // {
                    //     FD_CLR(i, &m_master_set);
                    //     close(i);
                    //     INFOS("Сокет закрылся со стороны клиента\n");
                    // }
                    // // обрабатываем полученное сообщение
                    // else
                    // {
                    //     memset(buffer + nread, 0, buf_size - nread);

                    //     // печать сообщения
                    //     printf("from client: [%s]\n ", buffer);

                    //     memset(buffer, '\n', buf_size);
                    //     // чтение с консоли
                    //     nread = read(STDIN_FILENO, buffer, buf_size);

                    //     // отправка на сервер
                    //     write(i, buffer, nread);
                    //}
                }
            }
        }
    }
}
