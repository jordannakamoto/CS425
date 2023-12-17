#include <iostream>
#include <thread>
#include <string>
#include "Connection.h"
#include "HTTPRequest.h"
#include "HTTPResponse.h"

const uint16_t DefaultPort = 8132;
const size_t num_threads = std::thread::hardware_concurrency();
const char* root = "/home/faculty/shreiner/public_html/03";


void handleSession(Connection& connection) {
    while (true) {
        Session session(connection.accept());
        std::string msg;
        session >> msg;
        HTTPRequest request(msg);
        HTTPResponse response(request,root);
        session << response;
    }
}

int main(int argc, char* argv[]) {
    uint16_t port = argc > 1 ? std::stol(argv[1]) : DefaultPort;
    Connection connection(port);

    std::vector<std::thread> sessionThreads(num_threads);
    for (auto& thread : sessionThreads) {
        thread = std::thread(handleSession, std::ref(connection));
    }

    for (auto& thread : sessionThreads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    return 0;
}
