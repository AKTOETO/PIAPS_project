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
    : Socket(), m_callback([](Request &)
                           { INFOS("Базовый callback\n"); })
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

    // проверка возможносит принятия соединения
    int fd = accept(m_socket, addr, addrlen);
    if(fd == -1)
    {
        INFOS("Невозможно принять соединение (возможно сокет закрыт)\n");
        return nullptr;
    }

    pSSocket new_socket = std::make_unique<ServerSocket>(fd);
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
    do
    {
        char *buf = NULL;
        int len = 0;

        // читаем большой буфер данных
        status = recvalls(&buf, len);

        // создание запроса
        Request req;
        req.m_data.assign(buf, status);
        req.m_socket = this;
        free(buf);

        //  если пришел буфер нулевой длины - нужно закрыться
        if(req.m_data.length() == 0)
            m_should_be_deleted = 1;

        // отправка запроса на сервер
        m_callback(req);

        // выходим из цикла
        if(m_should_be_deleted)
            break;

        // ожидание получения ответа через condition varable
        // метод setData оповещает этот поток о появлении даннх для отправки клиенту
        {
            std::unique_lock<std::mutex> lock(m_response_mutex);
            m_response_cv.wait(lock);
            // отправляем ответ
            sendall(m_response.c_str(), m_response.length());
        }

    } while (status != 0);

    return nullptr;
}

void ServerSocket::setCallback(std::function<void(Request &)> &&funct)
{
    m_callback = std::move(funct);
}

void ServerSocket::setData(const std::string &str)
{
    // TODO сделать установку данных, полученныз от сервера
    {
        std::lock_guard<std::mutex> lock(m_response_mutex);
        INFOS("Установка данных для отправки клиенту\n");
        m_response = str;
        m_response_cv.notify_one();
    }
}
