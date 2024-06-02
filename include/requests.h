#ifndef REQUESTS_H
#define REQUESTS_H

#include "includes.h"
#include "nlohmann/json.hpp"
#include "error.h"

using namespace nlohmann::literals;

namespace Request
{
    // тип выполнения запроса
    enum class ExecStatus
    {
        DONE = 0,
        ERROR,
    };

// ошибочный json файл
#define JSON_ERROR(msg) "{\"m_type\":\"reply\",\"m_status\":1,\"m_data\":\"" msg "\"}";

    // проверка на ошибку
    inline bool isItError(const std::string& str)
    {
        nlohmann::json j = nlohmann::json::parse(str);
        return (j["m_type"] == "reply" && j["m_status"] == 1);
    }


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
    inline nlohmann::json getJsonFromStr(const std::string &str)
    {
        // получение json файла из строки
        nlohmann::json j;
        try
        {
            j = nlohmann::json::parse(str);
        }
        catch (const std::exception &e)
        {
            ERROR("Ошибка преобразования строки в json: %s\n", std::string(e.what()).c_str());
            return JSON_ERROR("неверная строка");
        }
        return j;
    }

    // получение объекта из json
    template <class Ty>
    inline Ty getObjFromJson(const nlohmann::json &js)
    {
        // получение структуры авторизации
        Ty auth;

        try
        {
            auth = js.get<Ty>();
        }
        catch (const std::exception &e)
        {
            ERROR("Ошибка преобразования json в объект: %s\n", std::string(e.what()).c_str());
        }
        return auth;
    }

    // получить json из объекта
    template <class Ty>
    inline nlohmann::json getJsonFromObj(const Ty &obj)
    {
        nlohmann::json j;
        try
        {
            j = obj;
        }
        catch (const std::exception &e)
        {
            ERROR("Ошибка преобразования объекта в json: %s\n", std::string(e.what()).c_str());
            return JSON_ERROR("невозможное преобразование объекта в json");
        }
        return j;
    }

    // получить строку из json
    inline std::string getStrFromJson(const nlohmann::json &js)
    {
        std::string str;
        try
        {
            str = js.dump();
        }
        catch (const std::exception &e)
        {
            ERROR("Ошибка преобразования json в строку: %s\n", std::string(e.what()).c_str());
            return JSON_ERROR("невозможное преобразование json в строку");
        }
        return str;
    }

} // namespace Request

#endif