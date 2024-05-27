#ifndef SOCKET_H
#define SOCKET_H

#include "inculdes.h"
#include "error.h"

class Socket
{
protected:
    // преобразрование ip адреса из строки в число
    void inetPton(const char *src, void *dst);

public:
    // // уникальный указатель на сокет
    // using pSocket = std::shared_ptr<Socket>;

    // создание сокета
    Socket(int domain, int type, int protocol);
    Socket(int descriptor);
    Socket();

    void creates(int domain, int type, int protocol);

    // удаление сокета
    virtual ~Socket();

    // // привязка сокета к адресу
    // void binds(const sockaddr *addr, socklen_t addrlen);
    // void binds(int port, uint32_t ip = INADDR_ANY);
    // void binds(int port, const char *ip);

    // // задание значения очереди подключаемых клиентов
    // void listens(int backlog);

    // // принимаем подключение по прослушивающему сокету
    // pSocket accepts(sockaddr *addr, socklen_t *addrlen);
    // pSocket accepts();

    // // отправляем запрос на подключение
    // void connects(const sockaddr *addr, socklen_t addrlen);
    // void connects(uint32_t ip, int port);
    // void connects(const char *ip, int port);

    // получение дескриптора сокета
    int getDescr() const;

    // отправка сообщения
    int sends(const char *buf, int buf_len = 1024, int flag = 0);
    int sendall(const char *buff, int buf_len, int flag = 0);

    // получение сообщения
    int recvs(char *buf, int buf_len = 1024, int flag = 0);
    int recvalls(char **buf, int &buf_len, int flag = 0);

    // вызов функции обработки соединения сокетом
    void *operator()();

    // // установка функции обработки соединения
    // void setProcessFunct(std::function<void *()>);

    // функция обработки логики
    virtual void *processLogic();

    // закрытие сокета
    void closes();

    // получение статуса удаления
    bool getDeleteStatus() const;

protected:
    int m_socket;                           // дескриптор сокета
    int m_domain;                           // домен сокета (ipv4 / ipv6)
    bool m_should_be_deleted;               // сокет должен быть удален
};

#endif // !SOCKET_H