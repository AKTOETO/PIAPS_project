#ifndef SOCKET_H
#define SOCKET_H

#include "includes.h"
#include "error.h"

class Socket
{
protected:
    // преобразрование ip адреса из строки в число
    void inetPton(const char *src, void *dst);

public:

    // создание сокета
    Socket(int domain, int type, int protocol);
    Socket(int descriptor);
    Socket();

    void creates(int domain, int type, int protocol);

    // удаление сокета
    virtual ~Socket();

    // получение дескриптора сокета
    int getDescr() const;

    // отправка сообщения
    int sends(const char *buf, int buf_len = 1024, int flag = 0);
    /// @brief Отправка большого буфера данных
    /// @param buff буфер данных
    /// @param buf_len его длина
    /// @param flag флаги для send
    /// @return количество отправленных байт
    int sendall(const char *buff, int buf_len, int flag = 0);

    // получение сообщения
    int recvs(char *buf, int buf_len = 1024, int flag = 0);
    /// @brief Получение большого блока данных в буфер
    /// @param buf буфер данных
    /// @param buf_len его длина 
    /// @param flag флаги для recv
    /// @return 0 - сокет закрылся, иначе число считанных байт
    int recvalls(char **buf, int &buf_len, int flag = 0);

    // вызов функции обработки соединения сокетом
    void *operator()();

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