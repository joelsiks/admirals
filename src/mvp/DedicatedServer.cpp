#include "MvpServer.hpp"

int main() {
    admirals::mvp::MvpServer server(60000, true);
    server.Start();
    bool stopServer = false;
    server.EnterServerLoop(stopServer);

    return EXIT_SUCCESS;
}