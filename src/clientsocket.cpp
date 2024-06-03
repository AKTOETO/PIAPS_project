#include "clientsocket.h"

ClientSocket::ClientSocket(int domain, int type, int protocol)
    : Socket(domain, type, protocol)
{
}

ClientSocket::ClientSocket(int descriptor)
    : Socket(descriptor)
{
}

ClientSocket::ClientSocket()
    : Socket()
{
}

ClientSocket::~ClientSocket()
{
}

void ClientSocket::connects(const sockaddr *addr, socklen_t addrlen)
{
    INFOS("Запрос соединения\n");
    int res = connect(m_socket, addr, addrlen);

    if (res == -1)
    {
        ERRORS("Не получилось отправить запрос на соединение\n");
        throw std::runtime_error("connect error");
    }
    else if (res == 0)
    {
        INFOS("Соединеие установлено\n");
    }
}

void ClientSocket::connects(uint32_t ip, int port)
{
    // структура хранения адреса ipv4
    sockaddr_in addr;

    // Записываем семейство адресов, порт сервера, адрес сервера.
    addr.sin_family = m_domain;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(ip);

    // вызов уже существующей функции connects
    connects((sockaddr *)&addr, sizeof(addr));
}

void ClientSocket::connects(const char *ip, int port)
{
    // структура хранения адреса ipv4
    sockaddr_in addr;

    // Записываем семейство адресов, порт сервера, адрес сервера.
    addr.sin_family = m_domain;
    addr.sin_port = htons(port);
    inetPton(ip, &addr.sin_addr);

    // вызов уже существующей функции binds
    connects((sockaddr *)&addr, sizeof(addr));
}

void *ClientSocket::processLogic()
{
    
    INFOS("Обработка сокета со стороны клиента\n");

    std::string str;
    while (std::getline(std::cin, str) && str != "end")
    {
        // 1. ждем создания события от клиента (метод setData тут нужен (как у сервера))
        // 2. отправляем серверу событие
        // 3. ждем ответ от сервера
        // 4. отправляем ответ на обработчик (чере вызов callback функции, как у сервера)
        // 5. повторям все с первого шага

        // создание события авторизации
        AuthEvent ev{.m_name = str, .m_password = "2134"};

        // создание строки из объекта
        auto js = Request::getJsonFromObj<AuthEvent>(ev);
        // try
        // {
        //     std::cout<<js.at("m_tbhjkype")<<"\n\n";
            
        // }
        // catch(const std::exception& e)
        // {
        //     std::cerr << e.what() << '\n';
        // }
        
        str = Request::getStrFromJson(js);   

        // отправляем сообщение с консоли на сервер
        sendall(str.c_str(), str.length());

        // читаем ответ сервера
        char *buf = NULL;
        int len;
        len = recvalls(&buf, len);

        // printf("server> [");
        write(1, buf, len);
        // printf("]\n");

        free(buf);
    }

    return nullptr;
}
