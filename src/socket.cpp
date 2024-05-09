#include "socket.h"

void Socket::inetPton(const char *src, void *dst)
{
    INFOS("Получение ipv4 в виде числа из строки\n");
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

void* Socket::standProcessing()
{
    INFOS("Обработка соединения...\n");
    return NULL;
}

Socket::Socket(int domain, int type, int protocol)
    : Socket()
{
    creates(domain, type, protocol);
}

Socket::Socket(int descriptor)
    : Socket()
{
    m_socket = descriptor;
    socklen_t domain_len = sizeof(m_domain);

    // получение домена
    if (getsockopt(m_socket, SOL_SOCKET, SO_DOMAIN, &m_domain, &domain_len) == 0)
    {
        if(m_domain == AF_INET)
            INFOS("Домен IPv4\n");
    }
    else
    {
        ERRORS("Узнать домен сокета не получилось\n");
        throw std::runtime_error("getsockopt error");
    }
}

Socket::Socket()
    : m_socket(-1), m_domain(-1)
{
    INFOS("Конструктор сокета\n");
    m_process_func = standProcessing;
}

void Socket::creates(int domain, int type, int protocol)
{
    INFOS("Создание сокета\n");
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
    INFOS("Закрепление ip за сокетом\n");
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
    INFOS("Установка очереди прослушивания\n");
    int res = listen(m_socket, backlog);

    if (res == -1)
    {
        ERRORS("Не получилось задать длину очереди подключаемых клиентов для сокета\n");
        throw std::runtime_error("listen error");
    }
}

Socket::pSocket Socket::accepts(sockaddr *addr, socklen_t *addrlen)
{
    INFOS("Установка соединения\n");
    pSocket new_socket = std::make_unique<Socket>(accept(m_socket, addr, addrlen));

    if (new_socket->m_socket == -1)
    {
        ERRORS("Не получилось принять подключение\n");
        throw std::runtime_error("accept error");
    }
    sockaddr_in client_addr = *(sockaddr_in *)addr;
    INFO("Новый сокет %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    return new_socket;
}

Socket::pSocket Socket::accepts()
{
    sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    return std::move(accepts((sockaddr *) &client_addr, &client_len));
}

void Socket::connects(const sockaddr *addr, socklen_t addrlen)
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

int Socket::getDescr() const
{
    return m_socket;
}

void Socket::sends(const char *buf, int buf_len)
{
    // отправка на сервер
    if (write(m_socket, buf, buf_len) == -1)
    {
        ERRORS("Ошибка отправки данных\n");
        throw std::runtime_error("write error");
    }
    else
    {
        INFO("Сообщение отправлено: [%s]\n", buf);
    }
}

void Socket::sendall(const char *buff, int buf_len)
{
    // TODO: добавить возможность отправлять и получать сообщения большего размера, чем буфер
    // путем деления сообщения на маленькие буферы до 1024 байт.
    // алгоритм похожий есть в лабе по ос
}

int Socket::recvs(char *buf, int buf_len)
{
    int nread = read(m_socket, buf, buf_len);
    if (nread == -1)
    {
        ERRORS("Ошибка при чтении\n");
        throw std::runtime_error("read error");
    }
    else
    {
        INFO("Сообщение считано: [%s]\n", buf);
    }
    return nread;
}

int Socket::recvalls(char *buf, int buf_len)
{
    // TODO: добавить возможность отправлять и получать сообщения большего размера, чем буфер
    // путем деления сообщения на маленькие буферы до 1024 байт.
    // алгоритм похожий есть в лабе по ос
    return 0;
}

void* Socket::operator()()
{
    return std::move(m_process_func());
}
