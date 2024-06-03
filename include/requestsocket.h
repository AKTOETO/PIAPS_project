#ifndef REQUESTSOCKET_H
#define REQUESTSOCKET_H

#include "socket.h"

class RequestSocket : public Socket
{
public:
    // структура запроса к серверу
    struct Request
    {
        // кто отправил запрос
        RequestSocket *m_socket;

        // сам запрос
        std::string m_data;
    };

    // уникальный указатель на сокет
    using pSSocket = std::shared_ptr<RequestSocket>;

    // создание сокета
    RequestSocket(int domain, int type, int protocol);
    RequestSocket(int descriptor);
    RequestSocket();

    // удаление сокета
    ~RequestSocket();

    // установка callback'а
    void setCallback(std::function<void(Request &)> &&funct);

    // установка данных после обработки
    void setData(const std::string &str);

protected:
    // отправка данных с ожиданием появляения этих данных
    void waitingSend();

protected:
    // callback на сервер
    std::function<void(Request &)> m_callback;

    // ожидание получения ответа от сервера
    std::condition_variable m_response_cv;

    // ответ с сервера
    std::string m_response;

    // блокировка строки запроса
    std::mutex m_response_mutex;
};

#endif // !REQUESTSOCKET_H