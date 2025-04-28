#include "connection.h"
#include "connectionclosedexception.h"
#include "protocolviolationexception.h"
#include "messageHandler.h"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

using std::string;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;

std::vector<std::string> commands {
    "unknown_com",
    "no_com_end",
    "missing_par",
    "no_par_value",
    "wrong_par",
    "exit"
    // ...
};

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

string inputCommand() {
	string input = "";

    cout << "Available commands are: \n\n";

    for (auto c : commands) {
        cout << c << endl;
    }
    cout << endl;

    cout << "Select a command: \n\n";

    while (cin >> input){ 
		if (std::find(commands.begin(), commands.end(), input) != commands.end()) {
            break;
        }
        cout << "Incorrect command. The available commands are: \n\n";
        for (auto c : commands) {
            cout << c << endl;
        }
        cout << endl;
	}

    return input;
}

int app(const std::shared_ptr<Connection>& conn, const MessageHandler mh) {

    cout << "\n\n-----------------------------------------------------------------------\n\n";
    string command = inputCommand();
    
    try {
        if (command == "unknown_com") {
            // undefined commandbyte
            mh.sendCode(conn, int(Protocol::PAR_STRING));
        } else if (command == "missing_par") {
            // no parameter given
            mh.sendCode(conn, int(Protocol::COM_CREATE_NG));
            mh.sendCode(conn, int(Protocol::COM_END));
        } else if (command == "no_par_value") {
            // correct parameter byte, but no value for parameter
            mh.sendCode(conn, int(Protocol::COM_LIST_ART));
            mh.sendCode(conn, int(Protocol::PAR_NUM));
            mh.sendCode(conn, int(Protocol::COM_END));
        } else if (command == "no_com_end") {
            mh.sendCode(conn, int(Protocol::COM_CREATE_NG));
        } else if (command == "wrong_par") {
            // wrong parameter byte
            mh.sendCode(conn, int(Protocol::COM_DELETE_NG));
            mh.sendCode(conn, int(Protocol::PAR_STRING));
        } else if (command == "exit") {
            cout << "Exiting. Thank you for using the client!\n";
            return(0);
        } else {
            cerr << "\n\n\n Incorrect command was accepted. Hopefully you never read this, check that inputCommand() works correctly.";
            exit(3);
        }
    } catch (ConnectionClosedException) {
        cerr << "The connection is closed. Exeiting the program";
        exit(3);
    }

    cout << "\nContinue operation? (y,n)\n";
    string exit_command = "";
    getline(cin >> std::ws,exit_command);
    while (true) {
        if (exit_command == "n") {
            cout << "Exiting. Thank you for using the client!\n";
            return(0);
        } else if (exit_command == "y") {
            return 1;
        } else {
            cout << "Continue operation? (y,n)\n";
            getline(cin >> std::ws,exit_command);
        }
    }
}

int main(int argc, char* argv[]) {

    std::shared_ptr<Connection> conn = setupConnection(argc, argv);
    MessageHandler mh;
    while (app(conn,mh)){}
}