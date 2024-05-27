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
    return nread;
}

int Socket::recvalls(char **buf, int &buf_len, int flag)
{
    // очищаем память
    if (buf != NULL && *buf != NULL)
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
            (*buf) = (char *)realloc((*buf), buf_len);

            if ((*buf) == NULL)
            {
                ERRORS("realloc error\n");
                throw std::runtime_error("realloc error");
            }
        }


        // читаем сообщение
        nread = recvs((*buf) + total, len);

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
            total += nread;
            INFO("Получен блок данных размером %d\n", nread);
        }

    } while (nread > 0);

    return total;
}

void *Socket::operator()()
{
    return processLogic();
}

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
