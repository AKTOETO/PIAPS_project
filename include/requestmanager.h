#ifndef REQUESTMANAGER_H
#define REQUESTMANAGER_H

#include "requests.h"
#include "error.h"

using namespace Request;

// управляющий запросами
class RequestManager
{
private:
    // обработка авторизации
    static std::string authProc(const nlohmann::json &j);

    // обработка ответа
    // static std::string

public:
    /// @brief Получение ответа на запрос от клиента
    /// @param str входной json запрос от клиента
    /// @return ответ на запрос в виде строки json
    static std::string getResponse(const std::string &str);

private:
    // получение определенной функции обработки запроса в зависимости от типа
    static std::unordered_map<std::string, std::function<std::string(const nlohmann::json &)>> m_req_proc_func;
};

#endif // !REQUESTMANAGER_H