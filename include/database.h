#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>

using std::string;

class Database {
    public:
        // virtual std::vector<Newsgroup> ListNewsgroups() = 0;
        virtual bool createNewsgroup(string& name) = 0;
        virtual bool deleteNewsgroup(int newsgroupId) = 0;
        
        // virtual std::vector<Article> ListArticles(Newsgroup ng) = 0;
        virtual bool createArticle(string& name) = 0;
        virtual bool deleteArticle(int articleId) = 0;
        // virtual Article getArticle(int articleId) = 0;
};

#endif