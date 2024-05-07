#include "socket.h"

void Socket::inetPton(const char *src, void *dst)
{
    int res = inet_pton(AF_INET, src, dst);

    if (res == 0)
    {
        ERROR("Переданный ip: [%s] должен быть в формате [xxx.xxx.xxx.xxx]\n", src);
        throw std::runtime_error("inet_pton src error");
    }
    else if (res == -1)
    {
        ERROR("Неверно задан ip для семейства %d\n", m_domain);
        throw std::runtime_error("inet_pton incorrect domain");
    }
}

Socket::Socket(int domain, int type, int protocol)
    : m_domain(domain)
{
    creates(domain, type, protocol);
}

void Socket::creates(int domain, int type, int protocol)
{
    m_domain = domain;

    // создаем сокет
    m_socket = socket(domain, type, protocol);

    // произошла ошибка
    if (m_socket == -1)
    {
        ERRORS("Не получилось создать сокет\n");
        throw std::runtime_error("socket error");
    }
}

Socket::~Socket()
{
    INFOS("Закрытие сокета\n");
    close(m_socket);
}

void Socket::binds(const sockaddr *addr, socklen_t addrlen)
{
    int res = bind(m_socket, addr, addrlen);

    // если произошла ошибка, сообщаем об этом
    if (res == -1)
    {
        ERRORS("Не получилось прикрепить сокет к адресу\n");
        throw std::runtime_error("bind error");
    }
}

void Socket::binds(int port, uint32_t ip)
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

void Socket::binds(int port, const char *ip)
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

void Socket::listens(int backlog)
{
    int res = listen(m_socket, backlog);

    if (res == -1)
    {
        ERRORS("Не получилось задать длину очереди подключаемых клиентов для сокета\n");
        throw std::runtime_error("listen error");
    }
}

void Socket::accepts(sockaddr *addr, socklen_t *addrlen, Socket *new_socket)
{
    new_socket->m_socket = accept(m_socket, addr, addrlen);

    if (new_socket->m_socket == -1)
    {
        ERRORS("Не получилось принять подключение\n");
        throw std::runtime_error("accept error");
    }
}

void Socket::connects(const sockaddr *addr, socklen_t addrlen)
{
    int res = connect(m_socket, addr, addrlen);

    if (res == -1)
    {
        ERRORS("Не получилось отправить запрос на соединение\n");
        throw std::runtime_error("connect error");
    }
}

void Socket::connects(uint32_t ip, int port)
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

void Socket::connects(const char *ip, int port)
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

int Socket::operator()()
{
    return m_socket;
}
