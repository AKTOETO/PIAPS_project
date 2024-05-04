#include "error.h"

// печатать информационные сообщения
bool g_use_info = 1;

// поиск имени файла
size_t findFileNamePos(const char* str)
{
    size_t len = strlen(str);
    while(--len > 0)
    {
        if(str[len] == '/')
        {
            return len + 1;
        }
    }
    return len + 1;
}

// печать справочной информации
void printLog(
    FILE *out_stream, int pid, long tid, const char* file, const char* func,
    int line, const char* type)
{
    if(strcmp(type, "ERROR") == 0)
        fprintf(out_stream, "[%s%s%s][pid: %d][tid: %ld][%s][%s][line:%4d][sys er: %s] MSG: ",
                ERROR_BOLD_RED, type, DEFAULT_WHITE, pid, tid,
                file + findFileNamePos(file),func, line, strerror(errno));

    else if(strcmp(type, "INFO") == 0)
        fprintf(out_stream, "[%s%s%s][pid: %d][tid: %ld][%s][%s][line:%4d] MSG: ",
            INFO_BOLD_YELLOW, type, DEFAULT_WHITE, pid, tid,
            file + findFileNamePos(file), func, line);

    else
        fprintf(out_stream, "\n\tНеизвестный тип ошибки <%s>\n", type);
}