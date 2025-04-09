#ifndef INMEMEORY_H
#define INMEMORY_H

#include <string>
#include <vector>
#include "article.h"
#include "newsgroup.h"


using std::string;

class inMemory: public Database
{
public:
    inMemory();
    std::vector<Newsgroup> listNewsgroups();
    bool addNewsgroup(const string n);
    bool removeNewsgroup(unsigned long long id);
    std::optional<Newsgroup> getNewsgroup(unsigned long long id) const;

    std::vector<Article> ListArticles(Newsgroup &ng);
    bool addArticle(const std::string &t, const std::string &a, const std::string &txt);
    bool removeArticle(unsigned long long id);
    std::optional<Article> getArticle(unsigned long long id);

private:
    std::map<unsigned long long, Newsgroup> newsgroups;
    std::unordered_set<std::string> newsgroupNames;
    unsigned long long nextNewsgroupID; // technically maximum here
};

#endif