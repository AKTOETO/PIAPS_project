#include "clientsocket.h"

ClientSocket::ClientSocket(int domain, int type, int protocol)
    : Socket(domain, type, protocol)
{
}

ClientSocket::ClientSocket(int descriptor)
    : Socket(descriptor)
{
}

ClientSocket::ClientSocket()
    : Socket()
{
}

ClientSocket::~ClientSocket()
{
}

void ClientSocket::connects(const sockaddr *addr, socklen_t addrlen)
{
    INFOS("Запрос соединения\n");
    int res = connect(m_socket, addr, addrlen);

    if (res == -1)
    {
        ERRORS("Не получилось отправить запрос на соединение\n");
        throw std::runtime_error("connect error");
    }
    else if (res == 0)
    {
        INFOS("Соединеие установлено\n");
    }
}

void ClientSocket::connects(uint32_t ip, int port)
{
    // структура хранения адреса ipv4
    sockaddr_in addr;

    // Записываем семейство адресов, порт сервера, адрес сервера.
    addr.sin_family = m_domain;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(ip);

    // вызов уже существующей функции connects
    connects((sockaddr *)&addr, sizeof(addr));
}

void ClientSocket::connects(const char *ip, int port)
{
    // структура хранения адреса ipv4
    sockaddr_in addr;

    // Записываем семейство адресов, порт сервера, адрес сервера.
    addr.sin_family = m_domain;
    addr.sin_port = htons(port);
    inetPton(ip, &addr.sin_addr);

    // вызов уже существующей функции binds
    connects((sockaddr *)&addr, sizeof(addr));
}

void *ClientSocket::processLogic()
{
    INFOS("Обработка сокета со стороны клиента\n");
    std::this_thread::sleep_for(1s);
    return nullptr;
}
