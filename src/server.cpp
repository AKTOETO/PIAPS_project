#include "server.h"

void Server::initServer()
{
    INFOS("Запуск сервера\n");

    // создаем сокет для прослушки подключений (IPv4, TCP, протокол по умолчанию)
    m_listen_socket.creates(AF_INET, SOCK_STREAM, 0);

    // выполняем привязку сокета к адресу
    m_listen_socket.binds(12321);

    // определяем длину очереди подключаемых клиентов для прослушивающего сокета
    m_listen_socket.listens(5);
}

Server::Server()
    : m_listen_socket(), m_server_addres({0})
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
        Socket client_socket;
        m_listen_socket.accepts((sockaddr *)&client, &client_len, &client_socket);

        // количество считанных байт
        ssize_t nread;

        // сообщение от клиента
        char buf[256];

        // читаем сообщение от клиента
        nread = read(client_socket(), buf, 256);

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
            // печать в консоль полученной информации от клиента
            write(1, buf, nread);

            // отправка сообщения клиенту
            const char *msg = "msg from server\n";
            write(client_socket(), msg, strlen(msg));
        }
    }

}
