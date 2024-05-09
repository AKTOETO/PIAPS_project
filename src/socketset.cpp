#include "socketset.h"

SocketSet::SocketSet()
    : m_max_socket_decr(0)
{
    INFOS("Создание множества сокетов\n");

    // обнуление системного множества сокетов
    FD_ZERO(&m_set);
}

SocketSet::SocketSet(SocketSet &set)
{
    m_all_sockets = set.m_all_sockets;
    m_max_socket_decr = set.m_max_socket_decr;
    m_set = set.m_set;
}

SocketSet &SocketSet::operator=(SocketSet &set)
{
    if(this == &set)
        return *this;

    m_all_sockets = set.m_all_sockets;
    m_max_socket_decr = set.m_max_socket_decr;
    m_set = set.m_set;

    return *this;
}

void SocketSet::add(Socket::pSocket socket)
{
    // добавляем сокет в множество всех соектов
    m_all_sockets[socket->getDescr()] = std::move(socket);

    // добавляем сокет в множество дескрипторов
    FD_SET(socket->getDescr(), &m_set);

    // изменяем максимальное значение дескриптора
    m_max_socket_decr = std::max(socket->getDescr(), m_max_socket_decr);
}

void SocketSet::del(int descriptor)
{
    // итератор на удаленный элемент
    auto it = m_all_sockets.find(descriptor);

    // если элемент существует, удаляем его
    if (it != m_all_sockets.end())
    {
        FD_CLR(it->first, &m_set);
        m_all_sockets.erase(it);
        // TODO надо изменить максимальное значение дескриптора
    }
}

void SocketSet::waitSock()
{
    int res = select(m_max_socket_decr + 1, &m_set, NULL, NULL, NULL);

    // если результат отрицательный, произошла ошибка
    if (res == -1)
    {
        ERRORS("Ошибка при ожидании запросов на сокетах\n");
        throw std::runtime_error("select error");
    }
}

bool SocketSet::isSocketActive(std::unordered_map<int, Socket::pSocket>::const_iterator it)
{
    return FD_ISSET(it->first, &m_set);
}

bool SocketSet::isSocketListening(std::unordered_map<int, Socket::pSocket>::const_iterator it)
{
    return it == m_all_sockets.begin();
}

const std::unordered_map<int, Socket::pSocket> &SocketSet::getSockets() const
{
    return m_all_sockets;
}

// Socket::pSocket SocketSet::updateAllSockets()
// {
//     auto it = m_all_sockets.begin();

//     // надо опросить слушающий сокет на предмет присоединения клиента
//     if(FD_ISSET(it->first, &m_set))
//     {
//         auto ret = ((Socket::pSocket *)(*(it++)->second)());
//         return std::move(*ret);
//     }

//     // если же активен не слушающий сокет, то проверяем остальные
//     for(;it != m_all_sockets.end(); it++)
//     {
//         if(FD_ISSET())
//     }

//     return nullptr;
// }
