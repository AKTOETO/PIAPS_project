#include "serversocket.h"

ServerSocket::ServerSocket(int domain, int type, int protocol)
    : Socket(domain, type, protocol)
{
}

ServerSocket::ServerSocket(int descriptor)
    : Socket(descriptor)
{
}

ServerSocket::ServerSocket()
    : Socket()
{
}

ServerSocket::~ServerSocket()
{
}

void ServerSocket::binds(const sockaddr *addr, socklen_t addrlen)
{
    INFOS("Закрепление ip за сокетом\n");
    int res = bind(m_socket, addr, addrlen);

    // если произошла ошибка, сообщаем об этом
    if (res == -1)
    {
        ERRORS("Не получилось прикрепить сокет к адресу\n");
        throw std::runtime_error("bind error");
    }
}

void ServerSocket::binds(int port, uint32_t ip)
{
    // структура хранения адреса ipv4
    sockaddr_in addr;

    // Записываем семейство адресов, порт сервера, адрес сервера.
    addr.sin_family = m_domain;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(ip);

    // вызов уже существующей функции binds
    binds((sockaddr *)&addr, sizeof(addr));
}

void ServerSocket::binds(int port, const char *ip)
{
    // структура хранения адреса ipv4
    sockaddr_in addr;

    // Записываем семейство адресов, порт сервера, адрес сервера.
    addr.sin_family = m_domain;
    addr.sin_port = htons(port);
    // addr.sin_addr.s_addr = htonl(ip);
    // inet_pton(AF_INET, ip, &addr.sin_addr);
    inetPton(ip, &addr.sin_addr);

    // вызов уже существующей функции binds
    binds((sockaddr *)&addr, sizeof(addr));
}

void ServerSocket::listens(int backlog)
{
    INFOS("Установка очереди прослушивания\n");
    int res = listen(m_socket, backlog);

    if (res == -1)
    {
        ERRORS("Не получилось задать длину очереди подключаемых клиентов для сокета\n");
        throw std::runtime_error("listen error");
    }
}

ServerSocket::pSSocket ServerSocket::accepts(sockaddr *addr, socklen_t *addrlen)
{
    INFOS("Установка соединения\n");
    pSSocket new_socket = std::make_unique<ServerSocket>(accept(m_socket, addr, addrlen));

    if (new_socket->m_socket == -1)
    {
        ERRORS("Не получилось принять подключение\n");
        throw std::runtime_error("accept error");
    }
    sockaddr_in client_addr = *(sockaddr_in *)addr;
    INFO("Новый сокет %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    return new_socket;
}

ServerSocket::pSSocket ServerSocket::accepts()
{
    sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    return std::move(accepts((sockaddr *)&client_addr, &client_len));
}

void *ServerSocket::processLogic()
{
    INFOS("Обработка сокета со стороны сервера\n");

    int status = 0;

    // пока сокет открыт, читаем буфер
    // do
    //{
    //    // recvalls()
    //
    //} while (status > 0);
    std::this_thread::sleep_for(1s);

    return nullptr;
}
