
#include "newsapp.h"
#include "protocol.h"

#include <iostream>

using std::string;
using std::vector;

NewsApp::NewsApp(std::unique_ptr<Database> db, MessageHandler mh) : database(std::move(db)), messageHandler(mh) {}

void NewsApp::processRequest(std::shared_ptr<Connection>& conn) {

    Protocol code = static_cast<Protocol>(messageHandler.recvCode(conn));
    switch (code) {
        case Protocol::COM_LIST_NG:
            listGroups(conn);
            break;
        case Protocol::COM_CREATE_NG:

            break;
        case Protocol::COM_DELETE_NG:
        
            break;
        case Protocol::COM_LIST_ART:
        
            break;
        case Protocol::COM_CREATE_ART:
        
            break;
        case Protocol::COM_DELETE_ART:
        
            break;
        case Protocol::COM_GET_ART:

            break;
        default:
            // TODO
            // UNDEFINED CODE -> DISCONNECT CLIENT
            // conn->~Connection() <- ??
            std::cerr << "Invalid Command. Disconnecting Client...\n";
            return;
    }
}

void NewsApp::listGroups(std::shared_ptr<Connection>& conn) {
    vector<Newsgroup> groups = database->listNewsgroups();
    // ...
}

void createGroup(std::shared_ptr<Connection>& conn, const string name) {

}

void NewsApp::deleteGroup(std::shared_ptr<Connection>& conn, int ngId) {

}

void NewsApp::listArticles(std::shared_ptr<Connection>& conn, Newsgroup &ng) {

}

void NewsApp::createArticle(std::shared_ptr<Connection>& conn, const string &title, const string &author, const string &txt, Newsgroup &ng) {

}

void NewsApp::deleteArticle(std::shared_ptr<Connection>& conn, int id, Newsgroup &ng) {

}