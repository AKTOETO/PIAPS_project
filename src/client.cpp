#include "client.h"
void Client::initClient()
{
    INFOS("Запуск клиента\n");

    // создали сокет
    m_socket.creates(AF_INET, SOCK_STREAM, 0);

    // отправляем запрос серверу
    m_socket.connects("127.0.0.1", 12321);
}

Client::Client()
{
    initClient();
}

void Client::Run()
{
    INFOS("Работа клиента\n");

    // запуск обработки логики на сокете
    m_ptr_logic = std::make_unique<std::thread>(m_socket);

    m_ptr_logic->join();



    // // количество прочтенных байт
    // int nread;

    // // размер буфера
    // constexpr int buf_size = 1024;

    // // буфер чтения
    // char buffer[buf_size] = {0};
    // bool is_working = true;

    // // читаем с консоли сообщения и отправялем их серверу
    // while(is_working)
    // {
    //     memset(buffer, '\n', buf_size);

    //     nread = read(STDIN_FILENO, buffer, buf_size);
    //     printf("Считано: [%s]\n", buffer);
    //     if(buffer[0] == '#')
    //         is_working = false;

    //     // отправка на сервер
    //     m_socket.sends(buffer, nread);
    //     memset(buffer, '\n', buf_size);
    //     //if(write(m_socket(), buffer, nread) == -1)
    //     //{
    //     //    ERRORS("Ошибка отправки данных\n");
    //     //    throw std::runtime_error("write error");
    //     //}

    //     // получение ответа:
    //     //nread = read(m_socket(), buffer, buf_size);
    //     nread = m_socket.recvs(buffer, buf_size);

    //     // печать ответа в консоль
    //     printf("from server: [%s]\n", buffer);
    // }

    // close(m_socket.getDescr());



    // // после успешного подключения отправляем сообщение серверу
    // const char *msg = "imma client\n";
    // write(m_socket(), msg, strlen(msg));

    // // читаем ответ сервера
    // // количество считанных байт
    // ssize_t nread;

    // // сообщение от клиента
    // char buf[256];

    // // читаем сообщение от клиента
    // nread = read(m_socket(), buf, 256);

    // // если же прочитать не получилось
    // if (nread == -1)
    // {
    //     ERRORS("Прочитать не получилось\n");
    //     throw std::runtime_error("read error");
    // }
    // // если пришло пустоее сообщение
    // else if (nread == 0)
    // {
    //     INFOS("Конец файла\n");
    // }
    // // печатаем в консоль 
    // else
    // {
    //     write(1, buf, nread);
    // }

}
