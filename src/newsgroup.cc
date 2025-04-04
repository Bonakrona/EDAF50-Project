#include <optional>
#include "newsgroup.h"
#include "article.h"
#include <iostream>

Newsgroup::Newsgroup(const std::string &n)
{
    name = n;
    nextArcticleID = 1;
}

bool Newsgroup::addArticle(const std::string &t, const std::string &a, const std::string &txt)
{
    articles[nextArcticleID] = Article(t, a, txt, nextArcticleID);
    articleTitles.insert(t);
    // std::cout << "Your article has been created with the ID:" << nextArcticleID << "\n";
    nextArcticleID++;
    return true;
};

bool Newsgroup::writeArticle()
{
    std::string title, author, text;

    std::cout << "Enter the articles title: ";
    std::getline(std::cin, title);

    std::cout << "Enter the articles author: ";
    std::getline(std::cin, author);

    std::cout << "Enter the article text and end with an empty line: \n";
    std::string line;
    while (true)
    {
        std::getline(std::cin, line);
        if (line.empty())
            break;
        text += line + "\n";
    };
    return addArticle(title, author, text);
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

std::vector<Article> Newsgroup::listArticles(Newsgroup &ng) const
{
    std::vector<Article> res;

    for (const auto &pair : articles)
    {
        res.push_back(pair.second);
    }
    
    return res;
}

std::string Newsgroup::get_name() const
{
    return name;
}