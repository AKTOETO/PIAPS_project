#ifndef REQUESTMANAGER_H
#define REQUESTMANAGER_H

#include "inculdes.h"

class RequestManager
{
private:
    static std::string getType(const std::string& str);

public:
    /// @brief Получение ответа на запрос от клиента
    /// @param str входной json запрос от клиента
    /// @return ответ на запрос 
    static std::string getResponse(const std::string& str);

private:

    // тип запроса. Он всегда идет первой строкой в json файле
    enum class EventType
    {
        NONE = 0,
        GETTYPE,
    };

    // получение определенной функции обработки запроса в зависимости от типа
    static std::unordered_map<RequestManager::EventType, std::function<std::string(const std::string &)>> m_req_proc_func;
};

#endif // !REQUESTMANAGER_H