#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

#include "socket.h"

class ServerSocket : public Socket
{
public:
    // структура запроса к серверу
    struct Request
    {
        // кто отправил запрос
        ServerSocket *m_socket;

        // сам запрос
        std::string m_data;
    };

    // уникальный указатель на сокет
    using pSSocket = std::shared_ptr<ServerSocket>;

    // создание сокета
    ServerSocket(int domain, int type, int protocol);
    ServerSocket(int descriptor);
    ServerSocket();

    // удаление сокета
    ~ServerSocket();

    // привязка сокета к адресу
    void binds(const sockaddr *addr, socklen_t addrlen);
    void binds(int port, uint32_t ip = INADDR_ANY);
    void binds(int port, const char *ip);

    // задание значения очереди подключаемых клиентов
    void listens(int backlog);

    // принимаем подключение по прослушивающему сокету
    pSSocket accepts(sockaddr *addr, socklen_t *addrlen);
    pSSocket accepts();

    // функция обработки логики
    virtual void *processLogic() override;

    // установка callback'а
    void setCallback(std::function<void(Request &)>&& funct);

    // установка данных после обработки
    void setData(const std::string& str);

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

#endif // !SERVERSOCKET_H