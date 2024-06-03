#include "background.h"

// запуск работы бэкграунда
void Background::Run()
{
    INFOS("Запуск бэкграунда\n");
    m_threads.emplace_back(&Background::consoleInput, this);
    m_threads.emplace_back(&Background::socketProcessing, this);
    m_threads[1].detach();
    m_threads.emplace_back(&Background::logicProcessing, this);
    m_threads[2].detach();
    m_threads[0].join();
}

Background::Background()
    : m_is_running(true)
{
}

Background::~Background()
{
}
