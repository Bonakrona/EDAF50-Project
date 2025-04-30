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
    "read_article",
    "help",
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

    while (cin >> input){ 
		if (std::find(commands.begin(), commands.end(), input) != commands.end()) {
            break;
        }
        cout << "Incorrect command. Type \"help\" to list the available commands.\n";
	}

    return input;
}

int app(const std::shared_ptr<Connection>& conn, const ClientMessenger& cm) {

    cout << "\nSelect a command: \n\n";
    string command = inputCommand();
    
    try {
        if (command == "list_newsgroups") {
            cm.listNewsgroups(conn);
        } else if (command == "create_newsgroup") {
            cm.createNewsgroup(conn);
        } else if (command == "delete_newsgroup") {
            cm.deleteNewsgroup(conn);
        } else if (command == "list_articles") {
            cm.listArticles(conn,0);
        } else if (command == "create_article") {
            cm.createArticle(conn);
        } else if (command == "delete_article") {
            cm.deleteArticle(conn);
        } else if (command == "read_article") {
            cm.getArticle(conn);
        } else if (command == "help") {
            cout << "The available commands are: \n\n";
            for (auto c : commands) {
                cout << c << "\n";
            }
        } else if (command == "exit") {
            cout << "Exiting. Thank you for using the client!\n";
            return(0);
        } else {
            cerr << "\n\n\n Incorrect command was accepted. Hopefully you never read this, check that inputCommand() works correctly.";
            exit(3);
        }
    } catch (ConnectionClosedException& e) {
        cerr << "The connection is closed.\n" << "Exeiting the program.\n";
        exit(3);
    } catch(ProtocolViolationException& e) {
        cerr << "Protocol violation exception caught: " << e.msg << "\n" << "Exeiting the program.\n";
        exit(3);
    } catch(std::exception& e) {
        cerr << "Exception caught: " << e.what() << "\n" << "Exeiting the program.\n";
        exit(3);
    }

   return(1);
}

int main(int argc, char* argv[]) {

    std::shared_ptr<Connection> conn = setupConnection(argc, argv);
    ClientMessenger cm;

    cout << "Available commands are: \n\n";

    for (auto c : commands) {
        cout << c << "\n";
    }

    while (app(conn,cm)){}
}