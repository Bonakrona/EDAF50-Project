#include <optional>
#include "newsgroup.h"
#include "article.h"
#include <iostream>

Newsgroup::Newsgroup(const std::string &n, const unsigned long long i)
{
    name = n;
    id = i;
    nextArcticleID = 1;
}

bool Newsgroup::addArticle(const std::string &t, const std::string &a, const std::string &txt)
{
    if(articleTitles.find(t) != articleTitles.end()){
        return false;
    }else{
        articles[nextArcticleID] = Article(t, a, txt, nextArcticleID);
        articleTitles.insert(t);
        //std::cout << "Your article has been created with the ID:" << nextArcticleID << "\n";
        nextArcticleID++;
        return true;
    }
};

std::optional<Article> Newsgroup::getArticle(unsigned long long id) const
{
    auto it = articles.find(id);
    if (it != articles.end())
    {
        return it->second;
    }
    else
    {
        return std::nullopt;
    }
};

bool Newsgroup::removeArticle(unsigned long long id)
{
    auto it = articles.find(id);
    if (it != articles.end())
    {
        articleTitles.erase(it->second.get_title());
        articles.erase(it);
        return true;
    }
    return false; // Article not found
};

std::vector<Article> Newsgroup::listArticles() const
{
    std::vector<Article> res;

    for (const auto &pair : articles)
    {
        res.push_back(pair.second);
    }
    
    return res;
}

unsigned long long Newsgroup::get_id() const
{
    return id;
}

std::string Newsgroup::get_name() const
{
    return name;
}