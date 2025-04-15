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
        // initializes a Server on 'port' with Database 'db'
        // NewsApp(Server* serv, Database* db);
        // NewsApp(MessageHandler mh, Database* db);
        NewsApp(std::unique_ptr<Database> db, MessageHandler mh);

        void process_request(std::shared_ptr<Connection>& conn);

        /* Used in main server loop? Or remove and outsource to main file? */
        void serve_client();

        vector<string> list_groups();
        // TODO: add suitable parameters
        // TODO: change return value from 'bool' to 'int'? (for error codes)
        bool create_group();
        bool delete_group();
        
        // TODO: add suitable parameters
        // TODO: change return value from 'bool' to 'int'? (for error codes)
        vector<string> list_articles();
        bool create_article();
        bool delete_article();
        
    private:
        std::unique_ptr<Database> database; // obs: must be pointer or ref when abstract class
        MessageHandler messageHandler;

};

#endif