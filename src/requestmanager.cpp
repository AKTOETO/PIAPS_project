#include "requestmanager.h"

std::string RequestManager::getType(const std::string &str)
{
    return std::string();
}

std::string RequestManager::getResponse(const std::string &str)
{
    // получение json файла из строки

    // получение типа из строки

    // передача типа в мап для получения функции обработки
    //return m_req_proc_func[];
    return "";
}

std::unordered_map<RequestManager::EventType, std::function<std::string(const std::string &)>> RequestManager::m_req_proc_func = 
{
    {RequestManager::EventType::GETTYPE, RequestManager::getType},
};