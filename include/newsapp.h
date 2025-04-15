#ifndef NEWSAPP_H
#define NEWSAPP_H

#include "database.h"
#include "messageHandler.h"

#include <string>
#include <vector>

using std::string;
using std::vector;

class NewsApp {
    public:
        NewsApp(std::unique_ptr<Database> db, MessageHandler mh);

        // facilitate commands via private methods, defined below
        // based on the protocol defined in "protocol.h"
        void processRequest(std::shared_ptr<Connection>& conn);

    private:
        void listGroups(std::shared_ptr<Connection>& conn);
        void createGroup(std::shared_ptr<Connection>& conn, const string name);
        void deleteGroup(std::shared_ptr<Connection>& conn, int ngId);
        
        void listArticles(std::shared_ptr<Connection>& conn, Newsgroup &ng);
        void createArticle(std::shared_ptr<Connection>& conn, const string &title, const string &author, const string &txt, Newsgroup &ng);
        void deleteArticle(std::shared_ptr<Connection>& conn, int id, Newsgroup &ng);

        std::unique_ptr<Database> database; // obs: must be pointer or ref when abstract class
        MessageHandler messageHandler;

};

#endif