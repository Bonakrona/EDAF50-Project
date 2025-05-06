#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>
#include <functional>

#include "article.h"
#include "newsgroup.h"

using std::string;

class Database
{
public:
    Database() = default;
    virtual ~Database() = default;
    virtual std::vector<Newsgroup> listNewsgroups() = 0;
    virtual bool addNewsgroup(const string n) = 0;
    virtual bool removeNewsgroup(unsigned long long id) = 0;
    virtual std::optional<std::reference_wrapper<Newsgroup>> getNewsgroup(unsigned long long id) = 0;

    virtual std::vector<Article> listNewsgroupsArticles(Newsgroup &ng) = 0;
    virtual bool addNewsgroupsArticle(const std::string &t, const std::string &a, const std::string &txt, Newsgroup &ng) = 0;
    virtual bool removeNewsgroupsArticle(unsigned long long id, Newsgroup &ng) = 0;
    virtual std::optional<Article> getNewsgroupsArticle(unsigned long long id, Newsgroup &ng) const = 0;
};

#endif