#ifndef REQUESTS_H
#define REQUESTS_H

#include "includes.h"
#include "nlohmann/json.hpp"
#include "error.h"

using namespace nlohmann::literals;

namespace Events
{
    // тип выполнения запроса
    enum class ExecStatus
    {
        DONE = 0,
        ERROR,
    };

    // тип события
    enum class Type
    {
        REPLY = 0,
        AUTH
    };

// ошибочный json файл
#define JSON_ERROR(msg) "{\"m_type\":\"reply\",\"m_status\":1,\"m_data\":\"" msg "\"}";

    // проверка на ошибку
    inline bool isItError(const std::string &str)
    {
        nlohmann::json j = nlohmann::json::parse(str);
        return (j["m_type"] == Type::REPLY && j["m_status"] == ExecStatus::ERROR);
    }


    // ответ с сообщением
    struct ReplyEvent
    {
        Type m_type = Type::REPLY;
        ExecStatus m_status;
        std::string m_data;
    };

    // создание метода to_json и from_json
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ReplyEvent, m_type, m_status, m_data);

    // структура запроса авторизации
    struct AuthEvent
    {
        Type m_type = Type::AUTH;
        std::string m_name;
        std::string m_password;
    };

    // создание метода to_json и from_json
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(AuthEvent, m_type, m_name, m_password);

    // тип события
    using Event = std::variant<ReplyEvent, AuthEvent>;


    // обработка json преобразования для структуры Event
    inline void to_json(nlohmann::json &j, const Event &e)
    {
        // Так обрабатываем все типы:
        // если в объекте события сейчас тип события: ответ
        if(std::holds_alternative<ReplyEvent>(e))
        {  
            // обрабатываем его соответствующи образом
            j = std::get<ReplyEvent>(e);
        }
        else if(std::holds_alternative<AuthEvent>(e))
            j = std::get<AuthEvent>(e);
    }
    inline void from_json(const nlohmann::json &j, Event &e)
    {
        // в зависимости от типа нужно ывзывать соответствующий конвертер json
        switch (Type(j["m_type"]))
        {
        case Type::AUTH:
            e = j.get<AuthEvent>();
            break;

        case Type::REPLY:
            e = j.get<ReplyEvent>();
            break;

        default:
            break;
        }
    }

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