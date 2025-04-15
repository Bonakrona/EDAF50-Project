
#include "newsapp.h"
#include "protocol.h"
#include "protocolviolationexception.h"

#include <iostream>

using std::string;
using std::vector;

// static cast wrapper, for readability
int code(Protocol code) {
    return static_cast<int>(code);
}

// check that server receives a 'COM_END' after a command, otherwise ProtocolViolationException
void checkCommandEnd(std::shared_ptr<Connection>& conn, MessageHandler &mh) {
    int com = mh.recvCode(conn);
    if (com != code(Protocol::COM_END)) {
        throw new ProtocolViolationException("No COM_END received after command code!");
    }
}

NewsApp::NewsApp(std::unique_ptr<Database> database, MessageHandler messageHandler) : db(std::move(database)), mh(messageHandler) {}

void NewsApp::processRequest(std::shared_ptr<Connection>& conn) {

    Protocol code = static_cast<Protocol>(mh.recvCode(conn));
    try {
        switch (code) {
            case Protocol::COM_LIST_NG: {
                listGroups(conn);
                break;
            }
            case Protocol::COM_CREATE_NG: {
                string name = mh.recvStringParameter(conn);
                checkCommandEnd(conn, mh);
                createGroup(conn, name);
                break;
            }
            case Protocol::COM_DELETE_NG: {
                int id = mh.recvIntParameter(conn);
                checkCommandEnd(conn, mh);
                deleteGroup(conn, id);
                break;
            }
            case Protocol::COM_LIST_ART: {
                int ngId = mh.recvIntParameter(conn);
                checkCommandEnd(conn, mh);
                listArticles(conn, ngId);
                break;
            }
            case Protocol::COM_CREATE_ART: {
                int ngId = mh.recvIntParameter(conn);
                string title = mh.recvStringParameter(conn);
                string author = mh.recvStringParameter(conn);
                string text = mh.recvStringParameter(conn);
                checkCommandEnd(conn, mh);
                createArticle(conn, title, author, text, ngId);
                break;
            }
            case Protocol::COM_DELETE_ART: {
                int ngId = mh.recvIntParameter(conn);
                int artId = mh.recvIntParameter(conn);
                checkCommandEnd(conn, mh);
                deleteArticle(conn, artId, ngId);
                break;
            }
            case Protocol::COM_GET_ART: {
                int ngId = mh.recvIntParameter(conn);
                int artId = mh.recvIntParameter(conn);
                checkCommandEnd(conn, mh);
                getArticle(conn, ngId, artId);
                break;
            }
            default:
                // TODO
                // UNDEFINED CODE -> DISCONNECT CLIENT
                // conn->~Connection() <- ??
                // throw new ProtocolViolationException("Invalid command. Disconnecting client...")
                break;
            }
            
        } catch (ProtocolViolationException e) {
            // TODO: DISCONNECT CLIENT ?
            std::cerr << "Invalid command. Disconnecting client...\n";
    }
}

void NewsApp::listGroups(std::shared_ptr<Connection>& conn) {
    vector<Newsgroup> groups = db->listNewsgroups();
    mh.sendCode(conn, code(Protocol::ANS_LIST_NG));
    mh.sendIntParameter(conn, groups.size());
    for (Newsgroup ng : groups) {
        // string ngLine = f"id. newsgroup""
        // TODO: SEND STRINGS AS "[ID]. [NEWSGROUP NAME]" ??
        mh.sendStringParameter(conn, ng.get_name());
    }
    mh.sendCode(conn, code(Protocol::ANS_END));
}

void NewsApp::createGroup(std::shared_ptr<Connection>& conn, const string name) {
    mh.sendCode(conn, code(Protocol::ANS_CREATE_NG));
    if (db->addNewsgroup(name)) {
        mh.sendCode(conn, code(Protocol::ANS_ACK));
    } else {
        mh.sendCode(conn, code(Protocol::ANS_NAK));
        mh.sendCode(conn, code(Protocol::ERR_NG_ALREADY_EXISTS));
    }
    mh.sendCode(conn, code(Protocol::ANS_END));
}

void NewsApp::deleteGroup(std::shared_ptr<Connection>& conn, int ngId) {
    mh.sendCode(conn, code(Protocol::ANS_DELETE_NG));
    if (db->removeNewsgroup(ngId)) {
        mh.sendCode(conn, code(Protocol::ANS_ACK));
    } else {
        mh.sendCode(conn, code(Protocol::ANS_NAK));
        mh.sendCode(conn, code(Protocol::ERR_NG_DOES_NOT_EXIST));
    }
    mh.sendCode(conn, code(Protocol::ANS_END));
}

void NewsApp::listArticles(std::shared_ptr<Connection>& conn, int ngId) {
    mh.sendCode(conn, code(Protocol::ANS_LIST_ART));
    auto optNewsgroup = db->getNewsgroup(ngId);

    if (optNewsgroup.has_value()) {
        mh.sendCode(conn, code(Protocol::ANS_ACK));
        auto articles = optNewsgroup.value().listArticles();
        mh.sendIntParameter(conn, articles.size());
        for (Article article : articles) {
            // string articleLine = f"id. article""
            // TODO: SEND STRINGS AS "[ID]. [ARTICLE TITLE]" ??
            mh.sendStringParameter(conn, article.get_title());
        }
    } else {
        mh.sendCode(conn, code(Protocol::ANS_NAK));
        mh.sendCode(conn, code(Protocol::ERR_NG_DOES_NOT_EXIST));
    }
    mh.sendCode(conn, code(Protocol::ANS_END));
}

void NewsApp::createArticle(std::shared_ptr<Connection>& conn, 
                            const string &title, 
                            const string &author, 
                            const string &txt, 
                            int ngId) {
    mh.sendCode(conn, code(Protocol::ANS_CREATE_ART));
    auto optNewsgroup = db->getNewsgroup(ngId);
    
    if (optNewsgroup.has_value()) {
        db->addNewsgroupsArticle(title, author, txt, optNewsgroup.value());
        mh.sendCode(conn, code(Protocol::ANS_ACK));
    } else {
        mh.sendCode(conn, code(Protocol::ANS_NAK));
        mh.sendCode(conn, code(Protocol::ERR_NG_DOES_NOT_EXIST));
    }
    mh.sendCode(conn, code(Protocol::ANS_END));
}

void NewsApp::deleteArticle(std::shared_ptr<Connection>& conn, int artId, int ngId) {
    mh.sendCode(conn, code(Protocol::ANS_DELETE_ART));

    auto optNewsgroup = db->getNewsgroup(ngId);
    if (!optNewsgroup.has_value()) {
        mh.sendCode(conn, code(Protocol::ANS_NAK));
        mh.sendCode(conn, code(Protocol::ERR_NG_DOES_NOT_EXIST));
        mh.sendCode(conn, code(Protocol::ANS_END));
        return;
    }
    
    if (!db->removeNewsgroupsArticle(artId, optNewsgroup.value())) {
        mh.sendCode(conn, code(Protocol::ANS_NAK));
        mh.sendCode(conn, code(Protocol::ERR_ART_DOES_NOT_EXIST));
    } else {
        mh.sendCode(conn, code(Protocol::ANS_ACK));
    }
    
    mh.sendCode(conn, code(Protocol::ANS_END));
}

void NewsApp::getArticle(std::shared_ptr<Connection>& conn, int ngId, int artId) {
    mh.sendCode(conn, code(Protocol::ANS_GET_ART));

    auto optNewsgroup = db->getNewsgroup(ngId);
    if (!optNewsgroup.has_value()) {
        mh.sendCode(conn, code(Protocol::ANS_NAK));
        mh.sendCode(conn, code(Protocol::ERR_NG_DOES_NOT_EXIST));
        mh.sendCode(conn, code(Protocol::ANS_END));
        return;
    }

    auto optArticle = db->getNewsgroupsArticle(artId, optNewsgroup.value());
    if (!optArticle.has_value()) {
        mh.sendCode(conn, code(Protocol::ANS_NAK));
        mh.sendCode(conn, code(Protocol::ERR_ART_DOES_NOT_EXIST));
        mh.sendCode(conn, code(Protocol::ANS_END));
        return;
    }
    
    auto article = optArticle.value();

    mh.sendCode(conn, code(Protocol::ANS_ACK));
    mh.sendStringParameter(conn, article.get_title());
    mh.sendStringParameter(conn, article.get_author());
    mh.sendStringParameter(conn, article.get_content());
    mh.sendCode(conn, code(Protocol::ANS_END));
}



