#include "clientsocket.h"

ClientSocket::ClientSocket(int domain, int type, int protocol)
    : RequestSocket(domain, type, protocol)
{
}

ClientSocket::ClientSocket(int descriptor)
    : RequestSocket(descriptor)
{
}

ClientSocket::ClientSocket()
    : RequestSocket()
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
    int len = 0;
    do
    {
        // 1. ждем создания события от клиента (метод setData тут нужен (как у сервера))
        // {
        //     std::unique_lock<std::mutex> lock(m_response_mutex);
        //     m_response_cv.wait(lock);
        //     // 2. отправляем серверу событие
        //     sendall(m_response.c_str(), m_response.length());
        // }
        waitingSend();

        // создание события авторизации
        // Events::AuthEvent ev{.m_name = str, .m_password = "2134"};

        // создание строки из объекта
        // auto js = Events::getJsonFromObj<Events::AuthEvent>(ev);
        // try
        // {
        //     std::cout<<js.at("m_tbhjkype")<<"\n\n";

        // }
        // catch(const std::exception& e)
        // {
        //     std::cerr << e.what() << '\n';
        // }

        // str = Events::getStrFromJson(js);

        // отправляем сообщение с консоли на сервер
        // sendall(str.c_str(), str.length());

        // 3. ждем ответ от сервера
        // читаем ответ сервера
        char *buf = NULL;
        len = recvalls(&buf, len);

        // 4. формируем запрос на клиентскую часть
        Request req;
        req.m_data.assign(buf, len);
        req.m_socket = this;
        free(buf);

        // 5. отправляем ответ на обработчик (чере вызов callback функции, как у сервера)
        m_callback(req);

    } while (len != 0 && !m_should_be_deleted);

    return nullptr;
}
