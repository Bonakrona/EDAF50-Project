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

int ClientMessenger::listNewsgroups(const std::shared_ptr<Connection>& conn) const {
    int numberOfng = 0;

    // Send command
    mh.sendCode(conn, static_cast<int>(Protocol::COM_LIST_NG));
    mh.sendCode(conn, static_cast<int>(Protocol::COM_END));
    
    // Receive responce and print
    int rStartCode = mh.recvCode(conn);
    if (rStartCode != static_cast<int>(Protocol::ANS_LIST_NG)) {
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
    }
    if (numberOfng != 0) {
        cout << "\n";
    }

    int rTerminateCode = mh.recvCode(conn);
    if (rTerminateCode != static_cast<int>(Protocol::ANS_END)) {
        throw ProtocolViolationException("Received incorrect terminating code when using listNewsgroups()");
    }
    return(numberOfng);
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
    if (rStartCode != static_cast<int>(Protocol::ANS_CREATE_NG)) {
        throw ProtocolViolationException("Received incorrect starting code when using createNewsgroup()");
    }
    
    int ack = mh.recvCode(conn);
    if (ack == static_cast<int>(Protocol::ANS_ACK)) {
        cout << "Newsgroup was created with name \"" << ngName << "\".\n";
    } else {
        ack = mh.recvCode(conn);
        cout << "Newsgroup with name " << ngName << " already exists. No new newsgroup was created.\n"; 
    }

    int rTerminateCode = mh.recvCode(conn);
    if (rTerminateCode != static_cast<int>(Protocol::ANS_END)) {
        throw ProtocolViolationException("Received incorrect terminating code when using createNewsgroup()");
    }
    
}

void ClientMessenger::deleteNewsgroup(const std::shared_ptr<Connection>& conn) const {
    // List available newsgroups
    int numberOfng = listNewsgroups(conn);
    if (numberOfng == 0) {
        cout << "There are no newsgroups that can be deleted.\n";
        return;
    }

    // Enter name
    cout << "\nEnter ID of newsgroup to delete:\n";
    int ngID = inputID();

    // Send command
    mh.sendCode(conn,static_cast<int>(Protocol::COM_DELETE_NG));
    mh.sendIntParameter(conn,ngID);
    mh.sendCode(conn,static_cast<int>(Protocol::COM_END));

    // Receive response
    int rStartCode = mh.recvCode(conn);
    if (rStartCode != static_cast<int>(Protocol::ANS_DELETE_NG)) {
        throw ProtocolViolationException("Received incorrect starting code when using deleteNewsgroup()");
    }
    
    int ack = mh.recvCode(conn);
    if (ack == static_cast<int>(Protocol::ANS_ACK)) {
        cout << "Newsgroup was deleted with ID: " << ngID << ".\n";
    } else {
        ack = mh.recvCode(conn);
        cout << "Newsgroup with ID " << ngID << " does not exists. No newsgroup was deleted.\n"; 
    }

    int rTerminateCode = mh.recvCode(conn);
    if (rTerminateCode != static_cast<int>(Protocol::ANS_END)) {
        throw ProtocolViolationException("Received incorrect terminating code when using deleteNewsgroup()");
    }
    
}

int ClientMessenger::listArticles(const std::shared_ptr<Connection>& conn, int ngID) const {
    int numberOfArticles = 0;

    if (ngID == 0) {
        // List available newsgroups
        int numberOfng = listNewsgroups(conn);
        if (numberOfng == 0) {
            cout << "Create a newsgroup before listing articles.\n";
            return(numberOfArticles);
        }

        // Input ID
        cout << "\nEnter ID of newsgroup to list articles for:\n";
        ngID = inputID();
    }

    // Send command
    mh.sendCode(conn, static_cast<int>(Protocol::COM_LIST_ART));
    mh.sendIntParameter(conn,ngID);
    mh.sendCode(conn, static_cast<int>(Protocol::COM_END));
        
    // Receive responce and print
    int rStartCode = mh.recvCode(conn);
    if (rStartCode != static_cast<int>(Protocol::ANS_LIST_ART)) {
        throw ProtocolViolationException("Received incorrect starting code when using listArticles()");
    }

    int ack = mh.recvCode(conn);
    if (ack == static_cast<int>(Protocol::ANS_ACK)) {
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
    } else if(ack == static_cast<int>(Protocol::ANS_NAK)) {
        ack = mh.recvCode(conn);
        cout << "Newsgroup with ID " << ngID << " does not exists\n";
    } else {
        throw ProtocolViolationException("Received incorrect server response when using listArticles()");
    }

    int rTerminateCode = mh.recvCode(conn);
    if (rTerminateCode != static_cast<int>(Protocol::ANS_END)) {
        throw ProtocolViolationException("Received incorrect terminating code when using listArticles()");
    }
    return(numberOfArticles);
}

void ClientMessenger::createArticle(const std::shared_ptr<Connection>& conn) const {
    // List available newsgroups
    int numberOfng = listNewsgroups(conn);
    if (numberOfng == 0) {
        cout << "Create a newsgroup before creating an article.\n";
        return;
    }

    // Input ID and article data
    cout << "\nEnter ID of newsgroup to create an article in:\n";
    int ngID = inputID();

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
    if (rStartCode != static_cast<int>(Protocol::ANS_CREATE_ART)) {
        throw ProtocolViolationException("Received incorrect starting code when using createArticle()");
    }
    
    int ack = mh.recvCode(conn);
    if (ack == static_cast<int>(Protocol::ANS_ACK)) {
        cout << "Article \"" << title << "\" was created in newsgroup with ID: " << ngID << ".";
    } else if (ack == static_cast<int>(Protocol::ANS_NAK)) {
        ack = mh.recvCode(conn);
        cout << "Newsgroup with ID " << ngID << " does not exist. No article was created."; 
    } else {
        throw ProtocolViolationException("Received incorrect server response when using createArticle()");
    }
    cout << "\n";

    int rTerminateCode = mh.recvCode(conn);
    if (rTerminateCode != static_cast<int>(Protocol::ANS_END)) {
        throw ProtocolViolationException("Received incorrect terminating code when using createArticle()");
    }
}

void ClientMessenger::deleteArticle(const std::shared_ptr<Connection>& conn) const {
    // List available newsgroups
    int numberOfng = listNewsgroups(conn);
    if (numberOfng == 0) {
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
    mh.sendCode(conn,static_cast<int>(Protocol::COM_DELETE_ART));
    mh.sendIntParameter(conn,ngID);
    mh.sendIntParameter(conn,articleID);
    mh.sendCode(conn,static_cast<int>(Protocol::COM_END));

    // Receive response
    int rStartCode = mh.recvCode(conn);
    if (rStartCode != static_cast<int>(Protocol::ANS_DELETE_ART)) {
        throw ProtocolViolationException("Received incorrect starting code when using deleteArticle()");
    }
    
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
    if (rTerminateCode != static_cast<int>(Protocol::ANS_END)) {
        throw ProtocolViolationException("Received incorrect terminating code when using deleteArticle()");
    }
}

void ClientMessenger::getArticle(const std::shared_ptr<Connection>& conn) const {
    // List available newsgroups
    int numberOfng = listNewsgroups(conn);
    if (numberOfng == 0) {
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
    mh.sendCode(conn,static_cast<int>(Protocol::COM_GET_ART));
    mh.sendIntParameter(conn,ngID);
    mh.sendIntParameter(conn,articleID);
    mh.sendCode(conn,static_cast<int>(Protocol::COM_END));

    // Receive response
    int rStartCode = mh.recvCode(conn);
    if (rStartCode != static_cast<int>(Protocol::ANS_GET_ART)) {
        throw ProtocolViolationException("Received incorrect starting code when using getArticle()");
    }
    
    int ack = mh.recvCode(conn);
    if (ack == static_cast<int>(Protocol::ANS_ACK)) {
        string title = mh.recvStringParameter(conn);
        string author = mh.recvStringParameter(conn);
        string text = mh.recvStringParameter(conn);

        cout << "Article with ID: " << articleID << " of newsgroup with ID: " << ngID <<" is:\n\n";
        cout << "Title: \"" << title << "\", by " << author << ":\n\"\"\n";
        cout << text;
        cout << "\"\"\n";

    } else if (ack == static_cast<int>(Protocol::ANS_NAK)) {
        int err = mh.recvCode(conn);
        if (err == static_cast<int>(Protocol::ERR_ART_DOES_NOT_EXIST)) {
            cout << "No article with ID: " << articleID << " exists in newsgroup with ID: " << ngID << ".\n";
        } if (err == static_cast<int>(Protocol::ERR_NG_DOES_NOT_EXIST)) {
            cout << "Newsgroup with ID " << ngID << " does not exist.\n";
        }
    }

    int rTerminateCode = mh.recvCode(conn);
    if (rTerminateCode != static_cast<int>(Protocol::ANS_END)) {
        throw ProtocolViolationException("Incorrect terminating code when using get_article()");
    }
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