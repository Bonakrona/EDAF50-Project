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
    std::vector<Newsgroup> listNewsgroups();
    bool addNewsgroup(const string n);
    bool removeNewsgroup(unsigned long long id);
    std::optional<Newsgroup> Database::getNewsgroup(unsigned long long id) const;

    virtual std::vector<Article> ListArticles(Newsgroup &ng) = 0;
    virtual bool addArticle(const std::string &t, const std::string &a, const std::string &txt) = 0;
    virtual bool removeArticle(unsigned long long id) = 0;
    virtual std::optional<Article> getArticle(unsigned long long id) = 0;

private:
    std::map<unsigned long long, Newsgroup> newsgroups;
    std::unordered_set<std::string> newsgroupNames;
    unsigned long long nextNewsgroupID; // technically maximum here
};

#endif