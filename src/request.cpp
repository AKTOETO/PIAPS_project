#include "requests.h"


// получение json из строки json
nlohmann::json getJsonFromStr(const std::string& str)
{
    // получение json файла из строки
    nlohmann::json j;
    try
    {
        j = str;
    }
    catch (const std::exception &e)
    {
        ERROR("Ошибка преобразования строки в json: %s\n", std::string(e.what()).c_str());
        return JSON_ERROR("неверная строка");
    }
    return j;
}

// получение объекта из json
template<class Ty>
Ty getObjFromJson(const nlohmann::json& js)
{
    // получение структуры авторизации
    Ty auth;

    try
    {
        auth = js.get<Ty>();
    }
    catch(const std::exception& e)
    {
        ERROR("Ошибка преобразования json в объект: %s\n", std::string(e.what()).c_str());
    }
    return auth;    
}

// получить json из объекта
template<class Ty>
nlohmann::json getJsonFromObj(const Ty& obj)
{
    nlohmann::json j;
    try
    {
        j = obj;
    }
    catch(const std::exception& e)
    {
        ERROR("Ошибка преобразования объекта в json: %s\n", std::string(e.what()).c_str());
        return JSON_ERROR("невозможное преобразование объекта в json");
    }
    return j;
}

// получить строку из json
std::string getStrFromJson(const nlohmann::json& js)
{
    std::string str;
    try
    {
        str = js.get<std::string>();
    }
    catch(const std::exception& e)
    {
        ERROR("Ошибка преобразования json в строку: %s\n", std::string(e.what()).c_str());
        return JSON_ERROR("невозможное преобразование json в строку");
    }
    return str;
}