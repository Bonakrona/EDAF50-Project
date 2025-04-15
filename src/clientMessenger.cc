#include <clientMessenger.h>
#include <messageHandler.h>
#include <protocol.h>
#include <protocolviolationexception.h>

#include <iostream>
#include <string>
#include <sstream>

using std::string;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;

ClientMessenger::ClientMessenger() {
    msg = MessageHandler();
}

void ClientMessenger::listNewsgroups(const Connection& conn) const {
    // Send command
    msg.sendCode(conn, static_cast<int>(Protocol::COM_LIST_NG));
    msg.sendCode(conn, static_cast<int>(Protocol::COM_END));
    
    // Receive responce and print
    int rcode;
    rcode = msg.recvCode(conn);
    /*
    if (rcode != static_cast<int>(Protocol::ANS_LIST_NG)) {
        throw ProtocolViolationException("Incorrect starting code when using listNewsgroups()");
    }
    */

    int numberOfng = msg.recvIntParameter(conn);
    cout << "There are " << numberOfng << " newsgroups: \n";
    for (int i = 0; i < numberOfng; ++i) {
        int ngID = msg.recvIntParameter(conn);
        string ngName = msg.recvStringParameter(conn);

        cout << endl << ngName << "with ID: " << ngID;
    }
    cout << endl;

    rcode = msg.recvCode(conn);
    /*
    if (rcode != static_cast<int>(Protocol::ANS_END)) {
        throw ProtocolViolationException("Incorrect terminating code when using listNewsgroups()");
    }
    */
}

void ClientMessenger::createNewsgroup(const Connection& conn) const {
    // Enter name
    string ngName;
    cout << "Enter name of newsgroup to create:\n";
    cin >> ngName; 

    while (ngName.empty()) {
        cin >> ngName; 
    }

    // Send command
    msg.sendCode(conn,static_cast<int>(Protocol::COM_CREATE_NG));
    msg.sendStringParameter(conn,ngName);
    msg.sendCode(conn,static_cast<int>(Protocol::COM_END));

    // Receive response
    int rStartCode = msg.recvCode(conn);
    
    /*if (rStartCode != static_cast<int>(Protocol::ANS_CREATE_NG)) {
        throw ProtocolViolationException("Incorrect starting code when using createNewsgroup()");
    }
    */
    
    
    int ack = msg.recvCode(conn);

    if (ack == static_cast<int>(Protocol::ANS_ACK)) {
        cout << "Newsgroup was created with name \"" << ngName << "\".\n";
    } else {
        ack = msg.recvCode(conn);
        cout << "Newsgroup with name " << ngName << " already exists. No new newsgroup was created.\n"; 
    }

    int rTerminateCode = msg.recvCode(conn);
    /*
    if (rTerminateCode != static_cast<int>(Protocol::ANS_END)) {
        throw ProtocolViolationException("Incorrect terminating code when using createNewsgroup()");
    }
    */
}

void ClientMessenger::deleteNewsgroup(const Connection& conn) const {
    // Enter name
    int ngID = inputID("newsgroup");

    // Send command
    msg.sendCode(conn,static_cast<int>(Protocol::COM_DELETE_NG));
    msg.sendIntParameter(conn,ngID);
    msg.sendCode(conn,static_cast<int>(Protocol::COM_END));

    // Receive response
    int rStartCode = msg.recvCode(conn);
    /*if (rStartCode != static_cast<int>(Protocol::ANS_DELETE_NG)) {
        throw ProtocolViolationException("Incorrect starting code when using deleteNewsgroup()");
    }
    */

    int ack = msg.recvCode(conn);

    if (ack == static_cast<int>(Protocol::ANS_ACK)) {
        cout << "Newsgroup was deleted with ID \"" << ngID << "\".\n";
    } else {
        ack = msg.recvCode(conn);
        cout << "Newsgroup with ID " << ngID << " does not exists. No newsgroup was deleted.\n"; 
    }

    int rTerminateCode = msg.recvCode(conn);
    /*if (rTerminateCode != static_cast<int>(Protocol::ANS_END)) {
        throw ProtocolViolationException("Incorrect terminating code when using deleteNewsgroup()");
    }
    */
}

void ClientMessenger::listArticles(const Connection& conn) const {
    // Input ID
    string IDType = "newsgroup";
    int ngID = inputID(IDType);

    // Send command
    msg.sendCode(conn, static_cast<int>(Protocol::COM_LIST_ART));
    msg.sendIntParameter(conn,ngID);
    msg.sendCode(conn, static_cast<int>(Protocol::COM_END));
        
    // Receive responce and print
    int rcode;
    rcode = msg.recvCode(conn);
    /*
    if (rcode != static_cast<int>(Protocol::ANS_LIST_ART)) {
        throw ProtocolViolationException("Incorrect starting code when using listNewsgroups()");
    }
    */

    int ack = msg.recvCode(conn);

    if (ack == static_cast<int>(Protocol::ANS_ACK)) {

    }

    int numberOfng = msg.recvIntParameter(conn);
    cout << "There are " << numberOfng << " newsgroups: \n";
    for (int i = 0; i < numberOfng; ++i) {
        int ngID = msg.recvIntParameter(conn);
        string ngName = msg.recvStringParameter(conn);

        cout << endl << ngName << "with ID: " << ngID;
    }
    cout << endl;

    rcode = msg.recvCode(conn);
    /*
    if (rcode != static_cast<int>(Protocol::ANS_END)) {
        throw ProtocolViolationException("Incorrect terminating code when using listNewsgroups()");
    }
    */
    cout << "\n\n listArticles is incomplete \n\n";
}

void ClientMessenger::createArticle(const Connection& conn) const {
    cout << "\n\n createArticle is incomplete \n\n";
}

void ClientMessenger::deleteArticle(const Connection& conn) const {
    cout << "\n\n deleteArticle is incomplete \n\n";
}

void ClientMessenger::getArticle(const Connection& conn) const {
    cout << "\n\n getArticle is incomplete \n\n";
}

int ClientMessenger::inputID(const string& IDtype) const {
    string ngStr = "";

    cout << "Enter " << IDtype <<" ID to delete:\n";
    getline(cin >> std::ws,ngStr);

    while (!(ngStr.find_first_not_of("0123456789") == string::npos)) {
        cout << "ID must be singular interger:\n";
        getline(cin >> std::ws,ngStr);
        
    }
    return std::stoi(ngStr);
}