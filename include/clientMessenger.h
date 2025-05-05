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

        /* Runs the client application. */
        void runApp(const std::shared_ptr<Connection>& conn) const;

    private:

        /* Prompts the user to input a command executes the corresponding action. */
        int app(const std::shared_ptr<Connection>& conn) const;

        /* List the available newsgroups and returns a vector containing their IDs. */
        std::vector<int> listNewsgroups(const std::shared_ptr<Connection>& conn) const;

        /* Creates a newsgroup with a name given in the console. */
        void createNewsgroup(const std::shared_ptr<Connection>& conn) const;

        /* Deletes a newsgroup with the ID given in the console. */
        void deleteNewsgroup(const std::shared_ptr<Connection>& conn) const;

        /* Lists all articles in a newsgroups and returns the number of articles.*/
        int listArticles(const std::shared_ptr<Connection>& conn, int ngID) const;

        /* Creates an article in a newsgroup. */
        void createArticle(const std::shared_ptr<Connection>& conn) const;

        /* Deletes an article in a newsgroup. */
        void deleteArticle(const std::shared_ptr<Connection>& conn) const;

        /* Prints the data of an article in a newsgroup. */
        void getArticle(const std::shared_ptr<Connection>& conn) const;
        
        MessageHandler mh;

        /* List of commands. */
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

        /* Function that is used when the user inputs a command. */
        std::string inputCommand() const;

        /* Function that is used when the user inputs an ID. */
        int inputID() const;
};

#endif  