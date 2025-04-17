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
    mh = MessageHandler();
}

void ClientMessenger::listNewsgroups(const std::shared_ptr<Connection>& conn) const {
    // Send command
    mh.sendCode(conn, static_cast<int>(Protocol::COM_LIST_NG));
    mh.sendCode(conn, static_cast<int>(Protocol::COM_END));
    
    // Receive responce and print
    int rStartCode = mh.recvCode(conn);
    /*
    if (rcode != static_cast<int>(Protocol::ANS_LIST_NG)) {
        throw ProtocolViolationException("Incorrect starting code when using listNewsgroups()");
    }
    */

    int numberOfng = mh.recvIntParameter(conn);
    cout << "There are " << numberOfng << " newsgroups: \n";
    for (int i = 0; i < numberOfng; ++i) {
        int ngID = mh.recvIntParameter(conn);
        string ngName = mh.recvStringParameter(conn);

        cout << "\n Name: " << ngName << ", with ID: " << ngID;
    }

    int rTerminateCode = mh.recvCode(conn);
    /*
    if (rcode != static_cast<int>(Protocol::ANS_END)) {
        throw ProtocolViolationException("Incorrect terminating code when using listNewsgroups()");
    }
    */
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
    mh.sendCode(conn,static_cast<int>(Protocol::COM_CREATE_NG));
    mh.sendStringParameter(conn,ngName);
    mh.sendCode(conn,static_cast<int>(Protocol::COM_END));

    // Receive response
    int rStartCode = mh.recvCode(conn);
    
    /*if (rStartCode != static_cast<int>(Protocol::ANS_CREATE_NG)) {
        throw ProtocolViolationException("Incorrect starting code when using createNewsgroup()");
    }
    */
    
    
    int ack = mh.recvCode(conn);

    if (ack == static_cast<int>(Protocol::ANS_ACK)) {
        cout << "Newsgroup was created with name \"" << ngName << "\".\n";
    } else {
        ack = mh.recvCode(conn);
        cout << "Newsgroup with name " << ngName << " already exists. No new newsgroup was created.\n"; 
    }

    int rTerminateCode = mh.recvCode(conn);
    /*
    if (rTerminateCode != static_cast<int>(Protocol::ANS_END)) {
        throw ProtocolViolationException("Incorrect terminating code when using createNewsgroup()");
    }
    */
}

void ClientMessenger::deleteNewsgroup(const std::shared_ptr<Connection>& conn) const {
    // Enter name
    int ngID = inputID("newsgroup");

    // Send command
    mh.sendCode(conn,static_cast<int>(Protocol::COM_DELETE_NG));
    mh.sendIntParameter(conn,ngID);
    mh.sendCode(conn,static_cast<int>(Protocol::COM_END));

    // Receive response
    int rStartCode = mh.recvCode(conn);
    /*if (rStartCode != static_cast<int>(Protocol::ANS_DELETE_NG)) {
        throw ProtocolViolationException("Incorrect starting code when using deleteNewsgroup()");
    }
    */

    int ack = mh.recvCode(conn);

    if (ack == static_cast<int>(Protocol::ANS_ACK)) {
        cout << "Newsgroup was deleted with ID \"" << ngID << "\".\n";
    } else {
        ack = mh.recvCode(conn);
        cout << "Newsgroup with ID " << ngID << " does not exists. No newsgroup was deleted.\n"; 
    }

    int rTerminateCode = mh.recvCode(conn);
    /*if (rTerminateCode != static_cast<int>(Protocol::ANS_END)) {
        throw ProtocolViolationException("Incorrect terminating code when using deleteNewsgroup()");
    }
    */
}

void ClientMessenger::listArticles(const std::shared_ptr<Connection>& conn) const {
    // Input ID
    string IDType = "newsgroup";
    int ngID = inputID(IDType);

    // Send command
    mh.sendCode(conn, static_cast<int>(Protocol::COM_LIST_ART));
    mh.sendIntParameter(conn,ngID);
    mh.sendCode(conn, static_cast<int>(Protocol::COM_END));
        
    // Receive responce and print
    int rStartCode = mh.recvCode(conn);
    /*
    if (rcode != static_cast<int>(Protocol::ANS_LIST_ART)) {
        throw ProtocolViolationException("Incorrect starting code when using listNewsgroups()");
    }
    */

    int ack = mh.recvCode(conn);

    if (ack == static_cast<int>(Protocol::ANS_ACK)) {
        int numberOfArticles = mh.recvIntParameter(conn);
        cout << "There are " << numberOfArticles << " articles in newsgroup with ID: "<< ngID << ": \n";
        for (int i = 0; i < numberOfArticles; ++i) {
            int articleID = mh.recvIntParameter(conn);
            string articleName = mh.recvStringParameter(conn);

            cout << "\n Article name: \"" << articleName << ", with ID: " << articleID;
        }
    } else if(ack == static_cast<int>(Protocol::ANS_NAK)) {
        ack = mh.recvCode(conn);
        cout << "Newsgroup with ID " << ngID << " does not exists\n";
    } else {
        throw ProtocolViolationException("Incorrect server response for \"list_articles\"");
    }


    int rTerminateCode = mh.recvCode(conn);
    /*
    if (rcode != static_cast<int>(Protocol::ANS_END)) {
        throw ProtocolViolationException("Incorrect terminating code when using listNewsgroups()");
    }
    */
}

void ClientMessenger::createArticle(const std::shared_ptr<Connection>& conn) const {
    // Input newsgroup Id and article data
    string idTpye = "newsgroup";
    int ngID = inputID("newsgroup");

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
    mh.sendCode(conn,static_cast<int>(Protocol::COM_CREATE_ART));
    mh.sendIntParameter(conn,ngID);
    mh.sendStringParameter(conn,title);
    mh.sendStringParameter(conn,author);
    mh.sendStringParameter(conn,fullText.str());
    mh.sendCode(conn,static_cast<int>(Protocol::COM_END));

    // Receive response
    int rStartCode = mh.recvCode(conn);
    
    /*if (rStartCode != static_cast<int>(Protocol::uyaedwfuy)) {
        throw ProtocolViolationException("Incorrect starting code when using createNewsgroup()");
    }
    */
    
    int ack = mh.recvCode(conn);

    if (ack == static_cast<int>(Protocol::ANS_ACK)) {
        cout << "Article \"" << title << "\" was created in newsgroup with ID: " << ngID << ".";
    } else if (ack == static_cast<int>(Protocol::ANS_NAK)) {
        ack = mh.recvCode(conn);
        cout << "Newsgroup with ID " << ngID << " does not exist. No article was created."; 
    } else {
        throw ProtocolViolationException("Incorrect server response for \"create_article\"");
    }

    int rTerminateCode = mh.recvCode(conn);
}

void ClientMessenger::deleteArticle(const std::shared_ptr<Connection>& conn) const {
    // Input newsgroup and article IDs
    int ngID = inputID("newsgroup");

    int articleID = inputID("article");

    // Send message
    mh.sendCode(conn,static_cast<int>(Protocol::COM_DELETE_ART));
    mh.sendIntParameter(conn,ngID);
    mh.sendIntParameter(conn,articleID);
    mh.sendCode(conn,static_cast<int>(Protocol::COM_END));

    // Receive response
    int rStartCode = mh.recvCode(conn);
    
    /*if (rStartCode != static_cast<int>(Protocol::argag)) {
        throw ProtocolViolationException("Incorrect starting code when using createNewsgroup()");
    }
    */

    int ack = mh.recvCode(conn);

    if (ack == static_cast<int>(Protocol::ANS_ACK)) {
        cout << "Article with ID: " << articleID << " was deleted from newsgroup with ID: " << ngID << ".\n";
    } else if (ack == static_cast<int>(Protocol::ANS_NAK)) {
        int err = mh.recvCode(conn);
        if (err == static_cast<int>(Protocol::ERR_ART_DOES_NOT_EXIST)) {
            cout << "No article with ID: " << articleID << " exists in newsgroup with ID: " << ngID << ".\n";
        } if (err == static_cast<int>(Protocol::ERR_NG_DOES_NOT_EXIST)) {
            cout << "Newsgroup with ID " << ngID << " does not exist.\n";
        }
        cout << "No article was deleated.\n";
    }

    int rTerminateCode = mh.recvCode(conn);
    /*
    if (rcode != static_cast<int>(Protocol::ANS_END)) {
        throw ProtocolViolationException("Incorrect terminating code when using listNewsgroups()");
    }
    */

    //cout << "\n\n deleteArticle is incomplete \n\n";
}

void ClientMessenger::getArticle(const std::shared_ptr<Connection>& conn) const {
    // Input newsgroup and article IDs
    int ngID = inputID("newsgroup");

    int articleID = inputID("article");

    // Send message
    mh.sendCode(conn,static_cast<int>(Protocol::COM_GET_ART));
    mh.sendIntParameter(conn,ngID);
    mh.sendIntParameter(conn,articleID);
    mh.sendCode(conn,static_cast<int>(Protocol::COM_END));
    cout << "\n\n getArticle is incomplete \n\n";

    // Receive response
    int rStartCode = mh.recvCode(conn);
    
    /*if (rStartCode != static_cast<int>(Protocol::argag)) {
        throw ProtocolViolationException("Incorrect starting code when using createNewsgroup()");
    }
    */

    int ack = mh.recvCode(conn);

    if (ack == static_cast<int>(Protocol::ANS_ACK)) {
        string title = mh.recvStringParameter(conn);
        string author = mh.recvStringParameter(conn);
        string text = mh.recvStringParameter(conn);

        cout << "Article with ID: " << articleID << " of newsgroup with ID: " << ngID <<" is:\n\n";
        cout << "Title: \"" << title << "\", by \"" << author << "\",\n\n";
        cout << text;

    } else if (ack == static_cast<int>(Protocol::ANS_NAK)) {
        int err = mh.recvCode(conn);
        if (err == static_cast<int>(Protocol::ERR_ART_DOES_NOT_EXIST)) {
            cout << "No article with ID: " << articleID << " exists in newsgroup with ID: " << ngID << ".\n";
        } if (err == static_cast<int>(Protocol::ERR_NG_DOES_NOT_EXIST)) {
            cout << "Newsgroup with ID " << ngID << " does not exist.\n";
        }
    }
    int rTerminateCode = mh.recvCode(conn);
    /*
    if (rcode != static_cast<int>(Protocol::ANS_END)) {
        throw ProtocolViolationException("Incorrect terminating code when using listNewsgroups()");
    }
    */
}
/*
string ClientMessenger::inputStringWithoutSpaces(string& type) const {
    string strIn = "";

    cout << "Enter" << type << "\n";
    getline(cin >> std::ws,strIn);

    while (strIn.find(' ')<strIn.size()) {
        cout << "The " << type << "cannot include spaces:\n";
        getline(cin >> std::ws,strIn);
    }
    return strIn;
}
*/
int ClientMessenger::inputID(const string& IDtype) const {
    string IDStr = "";

    cout << "Enter " << IDtype <<" ID:\n";
    getline(cin >> std::ws,IDStr);

    while (!(IDStr.find_first_not_of("0123456789") == string::npos)) {
        cout << "ID must be singular interger:\n";
        getline(cin >> std::ws,IDStr);
        
    }
    return std::stoi(IDStr);
}