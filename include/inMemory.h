#ifndef INMEMEORY_H
#define INMEMORY_H

#include <string>
#include <vector>
#include "article.h"
#include "newsgroup.h"
#include "database.h"


using std::string;

class inMemory: public Database
{
public:
    inMemory();
    std::vector<Newsgroup> listNewsgroups();
    bool addNewsgroup(const string n);
    bool removeNewsgroup(unsigned long long id);
    std::optional<std::reference_wrapper<Newsgroup>> getNewsgroup(unsigned long long id);

    std::vector<Article> listNewsgroupsArticles(Newsgroup &ng);
    bool addNewsgroupsArticle(const std::string &t, const std::string &a, const std::string &txt, Newsgroup &ng);
    bool removeNewsgroupsArticle(unsigned long long id, Newsgroup &ng);
    std::optional<Article> getNewsgroupsArticle(unsigned long long id, Newsgroup &ng) const;

private:
    std::map<unsigned long long, Newsgroup> newsgroups;
    std::unordered_set<std::string> newsgroupNames;
    unsigned long long nextNewsgroupID; // technically maximum here
};

#endif