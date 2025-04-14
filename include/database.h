#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>
#include "article.h"
#include "newsgroup.h"

using std::string;

class Database
{
public:
    Database();
    virtual std::vector<Newsgroup> listNewsgroups() = 0;
    virtual bool addNewsgroup(const string n) = 0;
    virtual bool removeNewsgroup(unsigned long long id) = 0;
    virtual std::optional<Newsgroup> getNewsgroup(unsigned long long id) const = 0;

    virtual std::vector<Article> ListArticles(Newsgroup &ng) = 0;
    virtual bool addArticle(const std::string &t, const std::string &a, const std::string &txt, Newsgroup &ng) = 0;
    virtual bool removeArticle(unsigned long long id, Newsgroup &ng) = 0;
    virtual std::optional<Article> getArticle(unsigned long long id, Newsgroup &ng) const = 0;
};

#endif