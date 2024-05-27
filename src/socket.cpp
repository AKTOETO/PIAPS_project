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

// void *Socket::standProcessing()
// {
//     INFOS("Обработка соединения...\n");
//     std::this_thread::sleep_for(5s);

//     return NULL;
// }

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
        if (m_domain == AF_INET)
            INFOS("Домен IPv4\n");
    }
    else
    {
        ERRORS("Узнать домен сокета не получилось\n");
        throw std::runtime_error("getsockopt error");
    }
}

Socket::Socket()
    : m_socket(-1), m_domain(-1), m_should_be_deleted(0) 
{
    INFOS("Конструктор сокета\n");
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
    closes();
}

// void Socket::binds(const sockaddr *addr, socklen_t addrlen)
// {
//     INFOS("Закрепление ip за сокетом\n");
//     int res = bind(m_socket, addr, addrlen);

//     // если произошла ошибка, сообщаем об этом
//     if (res == -1)
//     {
//         ERRORS("Не получилось прикрепить сокет к адресу\n");
//         throw std::runtime_error("bind error");
//     }
// }

// void Socket::binds(int port, uint32_t ip)
// {
//     // структура хранения адреса ipv4
//     sockaddr_in addr;

//     // Записываем семейство адресов, порт сервера, адрес сервера.
//     addr.sin_family = m_domain;
//     addr.sin_port = htons(port);
//     addr.sin_addr.s_addr = htonl(ip);

//     // вызов уже существующей функции binds
//     binds((sockaddr *)&addr, sizeof(addr));
// }

// void Socket::binds(int port, const char *ip)
// {
//     // структура хранения адреса ipv4
//     sockaddr_in addr;

//     // Записываем семейство адресов, порт сервера, адрес сервера.
//     addr.sin_family = m_domain;
//     addr.sin_port = htons(port);
//     // addr.sin_addr.s_addr = htonl(ip);
//     // inet_pton(AF_INET, ip, &addr.sin_addr);
//     inetPton(ip, &addr.sin_addr);

//     // вызов уже существующей функции binds
//     binds((sockaddr *)&addr, sizeof(addr));
// }

// void Socket::listens(int backlog)
// {
//     INFOS("Установка очереди прослушивания\n");
//     int res = listen(m_socket, backlog);

//     if (res == -1)
//     {
//         ERRORS("Не получилось задать длину очереди подключаемых клиентов для сокета\n");
//         throw std::runtime_error("listen error");
//     }
// }

// Socket::pSocket Socket::accepts(sockaddr *addr, socklen_t *addrlen)
// {
//     INFOS("Установка соединения\n");
//     pSocket new_socket = std::make_unique<Socket>(accept(m_socket, addr, addrlen));

//     if (new_socket->m_socket == -1)
//     {
//         ERRORS("Не получилось принять подключение\n");
//         throw std::runtime_error("accept error");
//     }
//     sockaddr_in client_addr = *(sockaddr_in *)addr;
//     INFO("Новый сокет %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

//     return new_socket;
// }

// Socket::pSocket Socket::accepts()
// {
//     sockaddr_in client_addr;
//     socklen_t client_len = sizeof(client_addr);
//     return std::move(accepts((sockaddr *)&client_addr, &client_len));
// }

// void Socket::connects(const sockaddr *addr, socklen_t addrlen)
// {
//     INFOS("Запрос соединения\n");
//     int res = connect(m_socket, addr, addrlen);

//     if (res == -1)
//     {
//         ERRORS("Не получилось отправить запрос на соединение\n");
//         throw std::runtime_error("connect error");
//     }
//     else if (res == 0)
//     {
//         INFOS("Соединеие установлено\n");
//     }
// }

// void Socket::connects(uint32_t ip, int port)
// {
//     // структура хранения адреса ipv4
//     sockaddr_in addr;

//     // Записываем семейство адресов, порт сервера, адрес сервера.
//     addr.sin_family = m_domain;
//     addr.sin_port = htons(port);
//     addr.sin_addr.s_addr = htonl(ip);

//     // вызов уже существующей функции connects
//     connects((sockaddr *)&addr, sizeof(addr));
// }

// void Socket::connects(const char *ip, int port)
// {
//     // структура хранения адреса ipv4
//     sockaddr_in addr;

//     // Записываем семейство адресов, порт сервера, адрес сервера.
//     addr.sin_family = m_domain;
//     addr.sin_port = htons(port);
//     inetPton(ip, &addr.sin_addr);

//     // вызов уже существующей функции binds
//     connects((sockaddr *)&addr, sizeof(addr));
// }

int Socket::getDescr() const
{
    return m_socket;
}

int Socket::sends(const char *buf, int buf_len, int flag)
{
    int n = 0;

    // отправка на сервер
    if ((n = send(m_socket, buf, buf_len, flag)) == -1)
    {
        ERRORS("Ошибка отправки данных\n");
        throw std::runtime_error("write error");
    }

    INFO("Сообщение отправлено: [%s]\n", buf);
    return n;
}

int Socket::sendall(const char *buf, int buf_len, int flag)
{
    // TODO: добавить возможность отправлять и получать сообщения большего размера, чем буфер
    // путем деления сообщения на маленькие буферы до 1024 байт.
    // алгоритм похожий есть в лабе по ос

    // количество отправленных байт
    int total = 0;
    int n;

    // отправялем сообщения, пока не отправится весь буфер
    while (total < buf_len)
    {
        n = sends(buf + total, buf_len - total, flag);
        total += n;
    }

    return total;
}

int Socket::recvs(char *buf, int buf_len, int flag)
{
    int nread = recv(m_socket, buf, buf_len, flag);
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

int Socket::recvalls(char **buf, int &buf_len, int flag)
{
    // TODO: добавить возможность отправлять и получать сообщения большего размера, чем буфер
    // путем деления сообщения на маленькие буферы до 1024 байт.
    // алгоритм похожий есть в лабе по ос

    // очищаем память
    if (*buf != NULL)
    {
        delete[] *buf;
    }

    // длина буфера одного чтения
    const int len = 1024;

    //  текущий размер строки
    buf_len = 0;

    // всего получено байт
    int total = 0;

    // количество считанных байт за одно чтение
    int nread = 0;

    do
    {
        // если всего считано больше или равно длине строки,
        // увелчиваем память
        if (total >= buf_len)
        {
            buf_len += len;
            *buf = (char *)realloc(*buf, buf_len);
            if (*buf == NULL)
            {
                ERRORS("realloc error\n");
                throw std::runtime_error("realloc error");
            }
        }

        // читаем сообщение
        nread = recvs(*buf + buf_len, len);

        // если получили 0, значит сокет закрылся и нужно закрыть сокет
        if (nread == 0)
        {
            INFOS("Сокет закрылся\n");
            return 0;
        }
        // если получили меньше нуля, значит ошибка
        else if (nread < 0)
        {
            ERRORS("Ошибка при чтении всего\n");
            throw std::runtime_error("readall error");
        }
        // иначе получили блок данных
        else
        {
            buf_len += nread;
            INFO("Получен блок данных размером %d\n", nread);
        }

    } while (nread > 0);

    return 0;
}

void *Socket::operator()()
{
    return processLogic();
}

// void *Socket::procServer()
// {
//     INFOS("Обработка сокета со стороны сервера\n");

//     int status = 0;

//     // пока сокет открыт, читаем буфер
//     do
//     {
//         recvalls()


//     } while (status > 0);
    

//     return nullptr;
// }

// void *Socket::procClient()
// {
//     INFOS("Обработка сокета со стороны клиента\n");
//     return nullptr;
// }

void *Socket::processLogic()
{
    INFOS("Обработка соединения...\n");
    std::this_thread::sleep_for(5s);

    return nullptr;
}

void Socket::closes()
{
    INFOS("Закрытие сокета\n");
    m_should_be_deleted = 1;
    close(m_socket);
}

bool Socket::getDeleteStatus() const
{
    return m_should_be_deleted;
}
