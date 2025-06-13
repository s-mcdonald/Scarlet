#include <iostream>

#define LOCAL_PORT 8080
#define DEFAULT_PORT 80

// https://medium.com/@sakhawy/creating-an-http-server-from-scratch-ed41ef83314b
int main()
{
    // 1. Create socket
    std::cout << "ScarletServer attempt to create socket " << "...\n";

    // 2. Bind to port
    std::cout << "ScarletServer binding to port " << LOCAL_PORT << "...\n";

    // 3. Listen
    std::cout << "ScarletServer listening on port " << LOCAL_PORT << "...\n";

    // 4. Accept one connection
    std::cout << "ScarletServer accepting connection " << "...\n";

    // 5. Read request
    std::cout << "ScarletServer reading request " << "...\n";

    // 6. Send basic HTTP response
    std::cout << "ScarletServer sending response \n";

    // 7. Close
    std::cout << "ScarletServer shutting down "  << "...\n";
    return 0;
}