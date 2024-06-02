#include "server.h"

int main(int argc, char **argv)
{
    nlohmann::json js;

    try
    {
        Server server;
        server.Run();
    }
    catch (std::exception &e)
    {
        std::cout << "execption: " << e.what() << "\n";
    }
    return 0;
}
