#include "connection.h"
#include "connectionclosedexception.h"
#include "protocolviolationexception.h"
#include "messageHandler.h"
#include "clientMessenger.h"

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
    "list_newsgroups",
    "create_newsgroup",
    "delete_newsgroup",
    "list_articles",
    "create_article",
    "delete_article",
    "get_article",
    "exit"
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

int app(const std::shared_ptr<Connection>& conn, const ClientMessenger cm) {

    cout << "\n\n-----------------------------------------------------------------------\n\n";
    string command = inputCommand();
    
    try {
        if (command == "list_newsgroups") {
            cm.listNewsgroups(conn);
        } else if (command == "create_newsgroup") {
            cm.createNewsgroup(conn);
        } else if (command == "delete_newsgroup") {
            cm.deleteNewsgroup(conn);
        } else if (command == "list_articles") {
            cm.listArticles(conn);
        } else if (command == "create_article") {
            cm.createArticle(conn);
        } else if (command == "delete_article") {
            cm.deleteArticle(conn);
        } else if (command == "get_article") {
            cm.getArticle(conn);
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
    ClientMessenger cm;
    while (app(conn,cm)){}
}