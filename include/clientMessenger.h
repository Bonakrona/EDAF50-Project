#ifndef CLIENTMESSENGER_H
#define CLIENTMESSENGER_H

#include <string>

#include "connection.h"
#include "messageHandler.h"

class ClientMessenger {
    public:
        /* Creates the client messenger. */
        ClientMessenger();

        int listNewsgroups(const std::shared_ptr<Connection>& conn) const;

        void createNewsgroup(const std::shared_ptr<Connection>& conn) const;

        void deleteNewsgroup(const std::shared_ptr<Connection>& conn) const;

        int listArticles(const std::shared_ptr<Connection>& conn, int ngID) const;

        void createArticle(const std::shared_ptr<Connection>& conn) const;
        
        void deleteArticle(const std::shared_ptr<Connection>& conn) const;

        void getArticle(const std::shared_ptr<Connection>& conn) const;
        
    private:
        MessageHandler mh;

        int inputID() const;
};

#endif  