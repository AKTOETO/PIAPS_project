#ifndef SOCKET_H
#define SOCKET_H

#include "inculdes.h"
#include "error.h"

class Socket
{
private:
    // преобразрование ip адреса из чтроки в число
    void inetPton(const char *src, void *dst);

    // стандартная обработка соединения
    static void* standProcessing();

public:
    // уникальный указатель на сокет
    using pSocket = std::shared_ptr<Socket>;

    // создание сокета
    Socket(int domain, int type, int protocol);
    Socket(int descriptor);
    Socket();

    void creates(int domain, int type, int protocol);

    // удаление сокета
    ~Socket();

    // привязка сокета к адресу
    void binds(const sockaddr *addr, socklen_t addrlen);
    void binds(int port, uint32_t ip = INADDR_ANY);
    void binds(int port, const char *ip);

    // задание значения очереди подключаемых клиентов
    void listens(int backlog);

    // принимаем подключение по прослушивающему сокету
    pSocket accepts(sockaddr *addr, socklen_t *addrlen);
    pSocket accepts();

    // отправляем запрос на подключение
    void connects(const sockaddr *addr, socklen_t addrlen);
    void connects(uint32_t ip, int port);
    void connects(const char *ip, int port);

    // получение дескриптора сокета
    int getDescr() const;

    // отправка сообщения
    void sends(const char *buf, int buf_len = 1024);
    void sendall(const char *buff, int buf_len);

    // получение сообщения
    int recvs(char *buf, int buf_len = 1024);
    int recvalls(char *buf, int buf_len);

    // вызов функции обработки соединения сокетом
    void* operator()();

    // установка функции обработки соединения
    void setProcessFunct(std::function<void*()>);

private:
    int m_socket;                           // дескриптор сокета
    int m_domain;                           // домен сокета (ipv4 / ipv6)
    std::function<void*()> m_process_func;   // функция обработки соединения
};

#endif // !SOCKET_H