#include "connection.h"
#include "clientMessenger.h"

#include <cstdlib>
#include <iostream>

using std::cout;
using std::cerr;
using std::endl;

std::shared_ptr<Connection> setupConnection(int argc, char* argv[]) {

    if (argc != 3) {
        cerr << "Usage: client host-name port-number" << endl;
        exit(1);
    }

    int port = -1;
    try {
        port = std::stoi(argv[2]);
    } catch (std::exception& e) {
        cerr << "Wrong port number. " << e.what() << endl;
        exit(2);
    }

    std::shared_ptr<Connection> conn = std::make_shared<Connection>(argv[1], port);
    if (!(*conn).isConnected()) {
        cerr << "Connection attempt failed." << endl;
        exit(3);
    }

    cout << "Connected successfully." << endl;
    return conn;
}

int main(int argc, char* argv[]) {

    std::shared_ptr<Connection> conn = setupConnection(argc, argv);
    ClientMessenger cm;

    cm.runApp(conn);
}