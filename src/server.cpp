#include "server.h"

void Server::initServer()
{
    INFOS("Запуск сервера\n");

    // создаем сокет для прослушки подключений (IPv4, TCP, протокол по умолчанию)
    m_listen_socket = createSocket(AF_INET, SOCK_STREAM, 0);

    // Записываем семейство адресов, порт сервера, адрес сервера.
    m_server_addres.sin_family = AF_INET;
    m_server_addres.sin_port = htons(34543);
    // адрес
    //const char *addres = "192.168.0.209";
    //if(inet_aton(addres, &m_server_addres.sin_addr) != 1)
    //{
    //    ERROR("Не получилось получить ipv4 адрес: %s\n", addres);
    //    throw std::runtime_error("inet_pton error");
    //}
    //m_server_addres.sin_addr.s_addr = htonl(m_server_addres.sin_addr.s_addr);
    m_server_addres.sin_addr.s_addr = htonl(INADDR_ANY);

    // выполняем привязку сокета к адресу
    socketBind(m_listen_socket, (sockaddr *)&m_server_addres, sizeof(m_server_addres));

    // определяем длину очереди подключаемых клиентов для прослушивающего сокета
    socketListen(m_listen_socket, 5);
}

int Server::createSocket(int domain, int type, int protocol)
{
    // создаем сокет
    int res = socket(domain, type, protocol);

    // произошла ошибка
    if (res == -1)
    {
        ERRORS("Не получилось создать сокет\n");
        throw std::runtime_error("socket error");
    }

    // иначе возвращаем созданный сокет
    return res;
}

void Server::socketBind(int sockfd, const sockaddr *addr, socklen_t addrlen)
{
    int res = bind(sockfd, addr, addrlen);

    // если произошла ошибка, сообщаем об этом
    if (res == -1)
    {
        ERRORS("Не получилось прикрепить сокет к адресу\n");
        throw std::runtime_error("bind error");
    }
}

void Server::socketListen(int sockfd, int backlog)
{
    int res = listen(sockfd, backlog);

    if (res == -1)
    {
        ERRORS("Не получилось задать длину очереди подключаемых клиентов для сокета\n");
        throw std::runtime_error("listen error");
    }
}

int Server::socketAccept(int sockfd, sockaddr *addr, socklen_t *addrlen)
{
    int res = accept(sockfd, addr, addrlen);

    if (res == -1)
    {
        ERRORS("Не получилось принять подключение\n");
        throw std::runtime_error("accept error");
    }

    return res;
}

Server::Server()
    : m_listen_socket(0), m_server_addres({0})
{
    initServer();
}

void Server::Run()
{

    // пока что в бесконечном цикле принимаем подключения
    //for(;;)
    {
        // адрес клиента
        sockaddr_in client = m_server_addres;
        socklen_t client_len = sizeof(client);

        // подключаем клиента
        int client_socket = socketAccept(m_listen_socket, (sockaddr *)&client, &client_len);

        // количество считанных байт
        ssize_t nread;

        // сообщение от клиента
        char buf[256];

        // читаем сообщение от клиента
        nread = read(client_socket, buf, 256);

        // если же прочитать не получилось
        if(nread == -1)
        {
            ERRORS("Прочитать не получилось\n");
            throw std::runtime_error("read error");
        }
        // если пришло пустоее соощение
        else if(nread == 0)
        {
            INFOS("Конец файла\n");
        }
        else
        {
            write(1, buf, nread);
        }
    }

}
