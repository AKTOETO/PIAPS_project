#include "requestsocket.h"

RequestSocket::RequestSocket(int domain, int type, int protocol)
    : Socket(domain, type, protocol)
{
}

RequestSocket::RequestSocket(int descriptor)
    : Socket(descriptor)
{
}

RequestSocket::RequestSocket()
    : Socket(), m_callback([](Request &)
                           { INFOS("Базовый callback\n"); })
{
}

RequestSocket::~RequestSocket()
{
}

void RequestSocket::setCallback(std::function<void(Request &)> &&funct)
{
    m_callback = std::move(funct);
}

void RequestSocket::setData(const std::string &str)
{
    std::lock_guard<std::mutex> lock(m_response_mutex);
    INFOS("Установка данных\n");
    m_response = str;
    m_response_cv.notify_one();
}

void RequestSocket::waitingSend()
{
    std::unique_lock<std::mutex> lock(m_response_mutex);
    m_response_cv.wait(lock);
    sendall(m_response.c_str(), m_response.length());
}
