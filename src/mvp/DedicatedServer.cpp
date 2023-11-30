#include "MvpServer.hpp"

int main() {
    admirals::mvp::MvpServer server(60000);
    server.Start();
    server.EnterServerLoop();

    return EXIT_SUCCESS;
}