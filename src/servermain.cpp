#include "server.h"

int main(int argc, char **argv)
{
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
