#include <iostream>
#include "config.h"
#include "web_server.h"

int main() {
    Config config = load_config();

    // The main loop is now handled by the web server
    start_web_server();

    return 0;
}
