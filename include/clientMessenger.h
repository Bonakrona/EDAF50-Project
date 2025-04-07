#ifndef CLIENTMESSENGER_H
#define CLIENTMESSENGER_H

#include "connection.h"
#include "messageHandler.h"

class ClientMessenger {
    public:
        /* Creates the client messenger. */
        ClientMessenger() = default;

        void listNewsgroups(const MessageHandler& msg) const;

        void createNewsgroup(const MessageHandler& msg) const;

        void deleteNewsgroup(const MessageHandler& msg) const;

        void listArticles(const MessageHandler& msg) const;

        void createArticle(const MessageHandler& msg) const;
        
        void deleteArticle(const MessageHandler& msg) const;

        void getArticle(const MessageHandler& msg) const;
        
    protected:
};

#endif  