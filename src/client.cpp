#include "client.h"
void Client::initClient()
{
    INFOS("Запуск клиента\n");

    // создали сокет
    m_socket.creates(AF_INET, SOCK_STREAM, 0);
}

Client::Client()
{
    initClient();
}

void Client::Run()
{
    INFOS("Работа клиента\n");
    // отправляем запрос серверу
    m_socket.connects("127.0.0.1", 12321);

    // после успешного подключения отправляем сообщение серверу
    const char *msg = "imma client\n";
    write(m_socket(), msg, strlen(msg));

    // читаем ответ сервера
    // количество считанных байт
    ssize_t nread;

    // сообщение от клиента
    char buf[256];

    // читаем сообщение от клиента
    nread = read(m_socket(), buf, 256);

    // если же прочитать не получилось
    if (nread == -1)
    {
        ERRORS("Прочитать не получилось\n");
        throw std::runtime_error("read error");
    }
    // если пришло пустоее сообщение
    else if (nread == 0)
    {
        INFOS("Конец файла\n");
    }
    // печатаем в консоль 
    else
    {
        write(1, buf, nread);
    }
}
