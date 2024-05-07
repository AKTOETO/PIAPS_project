#include "client.h"
#include "iostream"

int main(int argc, char **argv)
{
    try
    {
        Client client;
        client.Run();
    }
    catch (std::exception &e)
    {
        std::cout << "execption: " << e.what() << "\n";
    }

    return 0;
}
