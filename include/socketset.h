#ifndef SOCKETSET_H
#define SOCKETSET_H

#include "socket.h"

class SocketSet
{
public:
    SocketSet();
    SocketSet(SocketSet &);

    SocketSet &operator=(SocketSet &);

    // добавление сокета в множество
    void add(Socket::pSocket socket);
    
    // удаление сокета из множества
    void del(int descriptor);

    // вычисление активных сокетов (select используем)
    void waitSock();

    // является ли сокет активным
    bool isSocketActive(std::unordered_map<int, Socket::pSocket>::const_iterator);

    // является ли сокет прослушивающим
    bool isSocketListening(std::unordered_map<int, Socket::pSocket>::const_iterator);

    // вернуть множество сокетов
    const std::unordered_map<int, Socket::pSocket> &getSockets() const;

private:
    fd_set m_set;                                           // множество дескрипторов сокетов
    std::unordered_map<int, Socket::pSocket> m_all_sockets; // <дескриптор, сокет> все сокеты во множестве
    int m_max_socket_decr;                                  // максимальный дескриптор в множестве
};

#endif // !SOCKETSET_H