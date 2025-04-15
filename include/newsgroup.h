#ifndef NEWSGROUP_H
#define NEWSGROUP_H

#include<iostream>
#include<vector>
#include<string>
#include<map>
#include<unordered_set>
#include<optional>
#include "article.h"
#include "database.h"

class Newsgroup{

public:
    //constructor
    Newsgroup() = default;
    Newsgroup(const std::string& n);

    bool addArticle(const std::string& t, const std::string& a, const std::string& txt);
    std::optional<Article> getArticle(unsigned long long id) const;
    bool removeArticle(unsigned long long id);
    std::vector<Article> listArticles() const;
    std::string get_name() const;

private:
    std::string name;
    std::map<unsigned long long, Article> articles;
    std::unordered_set<std::string> articleTitles;
    unsigned long long nextArcticleID; //technically maximum here
};

#endif