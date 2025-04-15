#include "connection.h"
#include "connectionclosedexception.h"
#include "server.h"
#include "newsapp.h"
#include "inMemory.h"
#include "messageHandler.h"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

using std::string;
using std::cout;
using std::cerr;
using std::endl;

Server init(int argc, char* argv[]) {
        if (argc != 2) {
                cerr << "Usage: myserver port-number" << endl;
                exit(1);
        }

        int port = -1;
        try {
                port = std::stoi(argv[1]);
        } catch (std::exception& e) {
                cerr << "Wrong format for port number. " << e.what() << endl;
                exit(2);
        }

        Server server(port);
        if (!server.isReady()) {
                cerr << "Server initialization error." << endl;
                exit(3);
        }
        return server;
}


void serve_client(Server& server, NewsApp& app) {
    auto conn = server.waitForActivity();
    if (conn != nullptr) {
            try {
                app.process_request(conn);
            } catch (ConnectionClosedException&) {
                    server.deregisterConnection(conn);
                    cout << "Client closed connection" << endl;
            }
    } else {
            conn = std::make_shared<Connection>();
            server.registerConnection(conn);
            cout << "New client connects" << endl;
    }
}

int main(int argc, char* argv[]) {
    auto IN_MEMORY = true;

    // initialize Server on port given in args
    Server server = init(argc, argv);

    // create (in-memory) Database + MessageHandler
    std::unique_ptr<Database> db = nullptr;
    if (IN_MEMORY)  {
        db = std::make_unique<inMemory>();
    } else {
        // TODO: after disk version of db is implemented
    }
    MessageHandler mh = MessageHandler();
    // initialize NewsApp
    NewsApp app = NewsApp(std::move(db), mh);

    // Main server loop:
    // OBS: Eventually switch to another condition?
    // (error handling, for example, etc..?, or just 'break')
    while (true) {
        serve_client(server, app);
    }

    return 0;
}
