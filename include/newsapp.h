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
        void process_request(std::shared_ptr<Connection>& conn);

    private:
    // TODO: change return values from 'bool' to 'int'? (for error codes)
        vector<string> listGroups();
        bool createGroup(const string name);
        bool deleteGroup(int ngId);
        
        vector<string> listArticles(Newsgroup &ng);
        bool createArticle(const string &title, const string &author, const string &txt, Newsgroup &ng);
        bool deleteArticle(int id, Newsgroup &ng);

        std::unique_ptr<Database> database; // obs: must be pointer or ref when abstract class
        MessageHandler messageHandler;

};

#endif