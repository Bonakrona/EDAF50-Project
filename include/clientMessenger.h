#ifndef CLIENTMESSENGER_H
#define CLIENTMESSENGER_H

#include <string>

#include "connection.h"
#include "messageHandler.h"

class ClientMessenger {
    public:
        /* Creates the client messenger. */
        ClientMessenger();

        void listNewsgroups(const Connection& conn) const;

        void createNewsgroup(const Connection& conn) const;

        void deleteNewsgroup(const Connection& conn) const;

        void listArticles(const Connection& conn) const;

        void createArticle(const Connection& conn) const;
        
        void deleteArticle(const Connection& conn) const;

        void getArticle(const Connection& conn) const;
        
    private:
        MessageHandler msg;

        int inputID(const std::string& IDtype) const;
};

#endif  