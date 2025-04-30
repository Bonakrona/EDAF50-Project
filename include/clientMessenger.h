#ifndef CLIENTMESSENGER_H
#define CLIENTMESSENGER_H

#include <string>
#include <vector>

#include "connection.h"
#include "messageHandler.h"

class ClientMessenger {
    public:
        /* Creates the client messenger. */
        ClientMessenger();

        void runApp(const std::shared_ptr<Connection>& conn) const;

    private:

        int app(const std::shared_ptr<Connection>& conn) const;

        int listNewsgroups(const std::shared_ptr<Connection>& conn) const;
        void createNewsgroup(const std::shared_ptr<Connection>& conn) const;
        void deleteNewsgroup(const std::shared_ptr<Connection>& conn) const;

        int listArticles(const std::shared_ptr<Connection>& conn, int ngID) const;
        void createArticle(const std::shared_ptr<Connection>& conn) const;
        void deleteArticle(const std::shared_ptr<Connection>& conn) const;
        void getArticle(const std::shared_ptr<Connection>& conn) const;
        
        MessageHandler mh;
        std::vector<std::string> commands {
            "list_newsgroups",
            "create_newsgroup",
            "delete_newsgroup",
            "list_articles",
            "create_article",
            "delete_article",
            "read_article",
            "help",
            "exit"
        };

        std::string inputCommand() const;
        int inputID() const;
};

#endif  