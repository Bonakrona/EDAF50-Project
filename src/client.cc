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
#include <set>

using std::string;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;

std::set<std::string> commands {
    "list_newsgroups",
    "create_newsgroup",
    "delete_newsgroup",
    "list_articles",
    "create_article",
    "delete_article",
    "get_article",
};

Connection setupConnection(int argc, char* argv[]) {

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

    Connection conn(argv[1], port);
    if (!conn.isConnected()) {
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
		if (commands.find(input) != commands.end()) {
            break;
        }
        cout << "Incorrect command. The available commands are: \n\n";
        for (auto c : commands) {
            cout << c << endl;
        }
        cout << endl;
	}

    return input;

    /*
	while (cin >> input){ // Maybe we should coose another word to terminate reading.
		if (!input.compare("&exit")) {
            break;
        }
        totalInput << input << "\n";

	}

    cout << endl << totalInput.str() << endl;

	return totalInput.str();
    */

}

int app(const Connection& conn) {

    ClientMessenger cm{};

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
        } else {
            cerr << "\n\n\n Incorrect command was accepted. Hopefully you never read this, check that inputCommand() works correctly.";
            exit(3);
        }
    } catch (ConnectionClosedException) {
        cerr << "The connection is closed. Exeiting the program";
        exit(3);
    }

    /*
    try {
        msg.sendStringParameter(command);
        string reply = msg.recvStringParameter();
    } catch (ProtocolViolationException) {
        cout << "\n\n Server is still not done. \n\n";
    }
    
    int nbr;
    while (cin >> nbr) {
        try {
                cout << nbr << " is ...";
                msg.sendInt(nbr);
                string reply = msg.recvStringParameter();
                cout << " " << reply << endl;
                cout << "Type another number: ";
        } catch (ConnectionClosedException&) {
                cout << " no reply from server. Exiting." << endl;
                return 1;
        }
    }
    */
    return 0;
}

int main(int argc, char* argv[]) {

    Connection conn = setupConnection(argc, argv);
    while (app(conn)){}
    
    return app(conn);
}