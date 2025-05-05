#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>

#include "clientMessenger.h"
#include "messageHandler.h"
#include "protocol.h"
#include "protocolviolationexception.h"
#include "connectionclosedexception.h"

using std::string;
using std::cin;
using std::cout;
using std::cerr;

// static cast wrapper, for readability
int code(Protocol code) {
    return static_cast<int>(code);
}

ClientMessenger::ClientMessenger() {}

void ClientMessenger::runApp(const std::shared_ptr<Connection>& conn) const {
    cout << "Available commands are: \n\n";

    for (auto c : commands) {
        cout << c << "\n";
    }

    while (app(conn)){}
}


int ClientMessenger::app(const std::shared_ptr<Connection>& conn) const {

    cout << "\nSelect a command: \n\n";
    string command = inputCommand();
    
    try {
        if (command == "list_newsgroups") {
            listNewsgroups(conn);
        } else if (command == "create_newsgroup") {
            createNewsgroup(conn);
        } else if (command == "delete_newsgroup") {
            deleteNewsgroup(conn);
        } else if (command == "list_articles") {
            listArticles(conn,0);
        } else if (command == "create_article") {
            createArticle(conn);
        } else if (command == "delete_article") {
            deleteArticle(conn);
        } else if (command == "read_article") {
            getArticle(conn);
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

std::unordered_set<int> ClientMessenger::listNewsgroups(const std::shared_ptr<Connection>& conn) const {
    int numberOfng = 0;
    std::unordered_set<int> newsgroupIDs;

    // Send command
    mh.sendCode(conn, code(Protocol::COM_LIST_NG));
    mh.sendCode(conn, code(Protocol::COM_END));
    
    // Receive responce and print
    int rStartCode = mh.recvCode(conn);
    if (rStartCode != code(Protocol::ANS_LIST_NG)) {
        throw ProtocolViolationException("Received incorrect starting code when using listNewsgroups()");
    }

    numberOfng = mh.recvIntParameter(conn);
    cout << "There are " << numberOfng << " newsgroups.";
    if (numberOfng != 0) {
        cout << " The available newsgroups are: \n";
    } else {
        cout << "\n";
    }
    for (int i = 0; i < numberOfng; ++i) {
        int ngID = mh.recvIntParameter(conn);
        string ngName = mh.recvStringParameter(conn);

        cout << "\n Name: \"" << ngName << "\", with ID: " << ngID;
        newsgroupIDs.insert(ngID);
    }
    if (numberOfng != 0) {
        cout << "\n";
    }

    int rTerminateCode = mh.recvCode(conn);
    if (rTerminateCode != code(Protocol::ANS_END)) {
        throw ProtocolViolationException("Received incorrect terminating code when using listNewsgroups()");
    }
    return(newsgroupIDs);
}

void ClientMessenger::createNewsgroup(const std::shared_ptr<Connection>& conn) const {
    // Enter name
    string ngName = "";
    cout << "Enter name of newsgroup to create:\n";
    getline(cin >> std::ws,ngName);

    while (ngName.empty()) {
        getline(cin >> std::ws,ngName);
    }

    // Send command
    mh.sendCode(conn,code(Protocol::COM_CREATE_NG));
    mh.sendStringParameter(conn,ngName);
    mh.sendCode(conn,code(Protocol::COM_END));

    // Receive response
    int rStartCode = mh.recvCode(conn);
    if (rStartCode != code(Protocol::ANS_CREATE_NG)) {
        throw ProtocolViolationException("Received incorrect starting code when using createNewsgroup()");
    }
    
    int ack = mh.recvCode(conn);
    if (ack == code(Protocol::ANS_ACK)) {
        cout << "Newsgroup was created with name \"" << ngName << "\".\n";
    } else {
        ack = mh.recvCode(conn);
        cout << "Newsgroup with name " << ngName << " already exists. No new newsgroup was created.\n"; 
    }

    int rTerminateCode = mh.recvCode(conn);
    if (rTerminateCode != code(Protocol::ANS_END)) {
        throw ProtocolViolationException("Received incorrect terminating code when using createNewsgroup()");
    }
    
}

void ClientMessenger::deleteNewsgroup(const std::shared_ptr<Connection>& conn) const {
    // List available newsgroups
    std::unordered_set<int> newsgroupIDs = listNewsgroups(conn);
    if (newsgroupIDs.empty()) {
        cout << "There are no newsgroups that can be deleted.\n";
        return;
    }

    // Enter name
    cout << "\nEnter ID of newsgroup to delete:\n";
    int ngID = inputID();

    // Send command
    mh.sendCode(conn,code(Protocol::COM_DELETE_NG));
    mh.sendIntParameter(conn,ngID);
    mh.sendCode(conn,code(Protocol::COM_END));

    // Receive response
    int rStartCode = mh.recvCode(conn);
    if (rStartCode != code(Protocol::ANS_DELETE_NG)) {
        throw ProtocolViolationException("Received incorrect starting code when using deleteNewsgroup()");
    }
    
    int ack = mh.recvCode(conn);
    if (ack == code(Protocol::ANS_ACK)) {
        cout << "Newsgroup was deleted with ID: " << ngID << ".\n";
    } else {
        ack = mh.recvCode(conn);
        cout << "Newsgroup with ID " << ngID << " does not exists. No newsgroup was deleted.\n"; 
    }

    int rTerminateCode = mh.recvCode(conn);
    if (rTerminateCode != code(Protocol::ANS_END)) {
        throw ProtocolViolationException("Received incorrect terminating code when using deleteNewsgroup()");
    }
    
}

int ClientMessenger::listArticles(const std::shared_ptr<Connection>& conn, int ngID) const {
    int numberOfArticles = 0;

    if (ngID == 0) {
        // List available newsgroups
        std::unordered_set<int> newsgroupIDs = listNewsgroups(conn);
        if (newsgroupIDs.empty()) {
            cout << "Create a newsgroup before listing articles.\n";
            return(numberOfArticles);
        }

        // Input ID
        cout << "\nEnter ID of newsgroup to list articles for:\n";
        ngID = inputID();
    }

    // Send command
    mh.sendCode(conn, code(Protocol::COM_LIST_ART));
    mh.sendIntParameter(conn,ngID);
    mh.sendCode(conn, code(Protocol::COM_END));
        
    // Receive responce and print
    int rStartCode = mh.recvCode(conn);
    if (rStartCode != code(Protocol::ANS_LIST_ART)) {
        throw ProtocolViolationException("Received incorrect starting code when using listArticles()");
    }

    int ack = mh.recvCode(conn);
    if (ack == code(Protocol::ANS_ACK)) {
        numberOfArticles = mh.recvIntParameter(conn);
        cout << "There are " << numberOfArticles << " articles in newsgroup with ID: "<< ngID << ".";
        if (numberOfArticles != 0) {
            cout << "The available articles are: \n";
        } else {
            cout << "\n";
        }
        for (int i = 0; i < numberOfArticles; ++i) {
            int articleID = mh.recvIntParameter(conn);
            string articleName = mh.recvStringParameter(conn);
            cout << "\n Name: \"" << articleName << "\", with ID: " << articleID;
        }
        if (numberOfArticles != 0) {
            cout << "\n";
        }
    } else if(ack == code(Protocol::ANS_NAK)) {
        ack = mh.recvCode(conn);
        cout << "Newsgroup with ID " << ngID << " does not exists\n";
    } else {
        throw ProtocolViolationException("Received incorrect server response when using listArticles()");
    }

    int rTerminateCode = mh.recvCode(conn);
    if (rTerminateCode != code(Protocol::ANS_END)) {
        throw ProtocolViolationException("Received incorrect terminating code when using listArticles()");
    }
    return(numberOfArticles);
}

void ClientMessenger::createArticle(const std::shared_ptr<Connection>& conn) const {
    // List available newsgroups
    std::unordered_set<int> newsgroupIDs = listNewsgroups(conn);
    if (newsgroupIDs.empty()) {
        cout << "Create a newsgroup before creating an article.\n";
        return;
    }

    // Input ID and article data
    cout << "\nEnter ID of newsgroup to create an article in:\n";
    int ngID = inputID();
    if (newsgroupIDs.find(ngID) == newsgroupIDs.end()) {
        cout << "Newsgroup with ID " << ngID << " does not exist. No article was created.";
        return;
    }

    string title = "";
    cout << "Enter the title of the article:\n";
    getline(cin >> std::ws,title);

    cout << "Enter the article's author:\n";
    string author = "";
    getline(cin >> std::ws,author);

    cout << "Enter article text. Mutiple lines can be entered, terminate the text by writing \"#EXIT#\" on an empty line.\n";
    std::stringstream fullText;
    string text = "";
    getline(cin >> std::ws,text);
    while (true){
        if (text.compare("#EXIT#") == 0) {
            break;
        } else {
            fullText << text << "\n";
            getline(cin >> std::ws,text);
        }
	}

    // Send message
    mh.sendCode(conn,code(Protocol::COM_CREATE_ART));
    mh.sendIntParameter(conn,ngID);
    mh.sendStringParameter(conn,title);
    mh.sendStringParameter(conn,author);
    mh.sendStringParameter(conn,fullText.str());
    mh.sendCode(conn,code(Protocol::COM_END));

    // Receive response
    int rStartCode = mh.recvCode(conn);
    if (rStartCode != code(Protocol::ANS_CREATE_ART)) {
        throw ProtocolViolationException("Received incorrect starting code when using createArticle()");
    }
    
    int ack = mh.recvCode(conn);
    if (ack == code(Protocol::ANS_ACK)) {
        cout << "Article \"" << title << "\" was created in newsgroup with ID: " << ngID << ".";
    } else if (ack == code(Protocol::ANS_NAK)) {
        ack = mh.recvCode(conn);
        cout << "Newsgroup with ID " << ngID << " does not exist. No article was created."; 
    } else {
        throw ProtocolViolationException("Received incorrect server response when using createArticle()");
    }
    cout << "\n";

    int rTerminateCode = mh.recvCode(conn);
    if (rTerminateCode != code(Protocol::ANS_END)) {
        throw ProtocolViolationException("Received incorrect terminating code when using createArticle()");
    }
}

void ClientMessenger::deleteArticle(const std::shared_ptr<Connection>& conn) const {
    // List available newsgroups
    std::unordered_set<int> newsgroupIDs = listNewsgroups(conn);
    if (newsgroupIDs.empty()) {
        cout << "Create a newsgroup with articles before deleteing an article.\n";
        return;
    }

    // Input newsgroup ID
    cout << "\nEnter ID of newsgroup to delete an article in:\n";
    int ngID = inputID();

    //List available articles
    int numberOfArticles = listArticles(conn,ngID);
    if (numberOfArticles == 0) {
        cout << "There are no articles that can be deleted.\n";
        return;
    }

    // Input article ID
    cout <<"\nEnter ID of article to delete:\n";
    int articleID = inputID();

    // Send message
    mh.sendCode(conn,code(Protocol::COM_DELETE_ART));
    mh.sendIntParameter(conn,ngID);
    mh.sendIntParameter(conn,articleID);
    mh.sendCode(conn,code(Protocol::COM_END));

    // Receive response
    int rStartCode = mh.recvCode(conn);
    if (rStartCode != code(Protocol::ANS_DELETE_ART)) {
        throw ProtocolViolationException("Received incorrect starting code when using deleteArticle()");
    }
    
    int ack = mh.recvCode(conn);
    if (ack == code(Protocol::ANS_ACK)) {
        cout << "Article with ID: " << articleID << " was deleted from newsgroup with ID: " << ngID << ".\n";
    } else if (ack == code(Protocol::ANS_NAK)) {
        int err = mh.recvCode(conn);
        if (err == code(Protocol::ERR_ART_DOES_NOT_EXIST)) {
            cout << "No article with ID: " << articleID << " exists in newsgroup with ID: " << ngID << ".\n";
        } if (err == code(Protocol::ERR_NG_DOES_NOT_EXIST)) {
            cout << "Newsgroup with ID " << ngID << " does not exist.\n";
        }
        cout << "No article was deleated.\n";
    }

    int rTerminateCode = mh.recvCode(conn);
    if (rTerminateCode != code(Protocol::ANS_END)) {
        throw ProtocolViolationException("Received incorrect terminating code when using deleteArticle()");
    }
}

void ClientMessenger::getArticle(const std::shared_ptr<Connection>& conn) const {
    // List available newsgroups
    std::unordered_set<int> newsgroupIDs = listNewsgroups(conn);
    if (newsgroupIDs.empty()) {
        cout << "Create a newsgroup with articles before reading an article.\n";
        return;
    }

    // Input newsgroup ID
    cout << "\nEnter ID of newsgroup to read an article from:\n";
    int ngID = inputID();

    //List available articles
    int numberOfArticles = listArticles(conn,ngID);
    if (numberOfArticles == 0) {
        cout << "There are no articles that can be read.\n";
        return;
    }

    // Input article ID
    cout <<"\nEnter ID of article to read:\n";
    int articleID = inputID();

    // Send message
    mh.sendCode(conn,code(Protocol::COM_GET_ART));
    mh.sendIntParameter(conn,ngID);
    mh.sendIntParameter(conn,articleID);
    mh.sendCode(conn,code(Protocol::COM_END));

    // Receive response
    int rStartCode = mh.recvCode(conn);
    if (rStartCode != code(Protocol::ANS_GET_ART)) {
        throw ProtocolViolationException("Received incorrect starting code when using getArticle()");
    }
    
    int ack = mh.recvCode(conn);
    if (ack == code(Protocol::ANS_ACK)) {
        string title = mh.recvStringParameter(conn);
        string author = mh.recvStringParameter(conn);
        string text = mh.recvStringParameter(conn);

        cout << "Article with ID: " << articleID << " of newsgroup with ID: " << ngID <<" is:\n\n";
        cout << "Title: \"" << title << "\", by " << author << ":\n\"\"\n";
        cout << text;
        cout << "\"\"\n";

    } else if (ack == code(Protocol::ANS_NAK)) {
        int err = mh.recvCode(conn);
        if (err == code(Protocol::ERR_ART_DOES_NOT_EXIST)) {
            cout << "No article with ID: " << articleID << " exists in newsgroup with ID: " << ngID << ".\n";
        } if (err == code(Protocol::ERR_NG_DOES_NOT_EXIST)) {
            cout << "Newsgroup with ID " << ngID << " does not exist.\n";
        }
    }

    int rTerminateCode = mh.recvCode(conn);
    if (rTerminateCode != code(Protocol::ANS_END)) {
        throw ProtocolViolationException("Incorrect terminating code when using get_article()");
    }
}

string ClientMessenger::inputCommand() const {
	string input = "";

    while (cin >> input){ 
		if (std::find(commands.begin(), commands.end(), input) != commands.end()) {
            break;
        }
        cout << "Incorrect command. Type \"help\" to list the available commands.\n";
	}

    return input;
}

int ClientMessenger::inputID() const {
    string IDStr = "";

    getline(cin >> std::ws,IDStr);

    while (!(IDStr.find_first_not_of("0123456789") == string::npos)) {
        cout << "ID must be singular interger:\n";
        getline(cin >> std::ws,IDStr);
        
    }
    return std::stoi(IDStr);
}