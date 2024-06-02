#include "requestmanager.h"

std::string RequestManager::authProc(const nlohmann::json &j)
{
    // // получение структуры авторизации
    // auto auth = j.get<AuthEvent>();
    AuthEvent auth = getObjFromJson<AuthEvent>(j);

    // TODO по-хорошему, тут должно быть образение к бд
    // и проверка на ней существования такого пользователя
    // но пока тут просто печать
    printf("name: [%s] password: [%s]\n", auth.m_name.c_str(), auth.m_password.c_str());

    // временная проверка авторизации
    auto authed = [&]()
    {
        return (auth.m_name == "admin" && auth.m_password == "2134");
    };

    // формирование ответа
    ReplyEvent rep{
        .m_status = (authed() ? ExecStatus::DONE : ExecStatus::ERROR),
        .m_data = ""};

    // создание json строки
    nlohmann::json out = rep;

    return out.dump();
}

std::string RequestManager::getResponse(const std::string &str)
{
    // получение json файла из строки
    nlohmann::json j = Request::getJsonFromStr(str);

    // если не было ошибки обработки пакета
    if (isItError(str))
    {
        ERROR("Некорректная обработка json: {%s}\n", j.dump().c_str());
        return JSON_ERROR("Некорректная обработка json");
    }

    // функция обработки текущего типа
    std::function<std::string(const nlohmann::json &)> funct;

    std::string type;
    // получение функции обработчика
    try
    {
        std::cout << std::setw(4) << j << std::endl;
        type = j.at("m_type");
        INFO("Полученный тип запроса: %s\n", type.c_str());
    }
    catch (const std::exception &e)
    {
        ERROR("Ошибка получения функции преобразования: %s\n", std::string(e.what()).c_str());
        return JSON_ERROR("Нет функции преобразования типа");
    }

    try
    {
        funct = m_req_proc_func.at(type);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    // обработка json файла
    return funct(j);
}

std::unordered_map<std::string, std::function<std::string(const nlohmann::json &)>> RequestManager::m_req_proc_func =
    {
        //{EventType::GETTYPE, RequestManager::getType},
        {"auth", RequestManager::authProc},
};