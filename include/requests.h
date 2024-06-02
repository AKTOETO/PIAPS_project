#ifndef REQUESTS_H
#define REQUESTS_H

#include "inculdes.h"
#include "nlohmann/json.hpp"
#include "error.h"

using namespace nlohmann::literals;

// тип выполнения запроса
enum class ExecStatus
{
    DONE = 0,
    ERROR,
};

// ошибочный json файл
#define JSON_ERROR(msg) "{\"m_type\":\"reply\",\"m_status\":1,\"m_data\":\"" msg "\"}"

// ответ с сообщением
struct ReplyEvent
{
    std::string m_type = "reply";
    ExecStatus m_status;
    std::string m_data;
};
// создание метода to_json и from_json
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ReplyEvent, m_type, m_status, m_data)

// структура запроса авторизации
struct AuthEvent
{
    std::string m_type = "auth";
    std::string m_name;
    std::string m_password;
};
// создание метода to_json и from_json
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(AuthEvent, m_type, m_name, m_password)


// получение json из строки json
nlohmann::json getJsonFromStr(const std::string& str);

// получение объекта из json
template<class Ty>
Ty getObjFromJson(const nlohmann::json& js);

// получить json из объекта
template<class Ty>
nlohmann::json getJsonFromObj(const Ty& obj);

// получить строку из json
std::string getStrFromJson(const nlohmann::json& js);

#endif // !REQUESTS_H