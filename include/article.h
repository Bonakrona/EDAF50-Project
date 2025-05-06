#ifndef ARTICLE_H
#define ARTICLE_H

#include <iostream>
#include <vector>
#include <string>

class Article {

public:
    Article() = default;
    Article(const std::string& t, const std::string& a, const std::string& txt, unsigned long long id);
    const std::string& get_author() const;
    const std::string& get_title() const;
    const std::string& get_content() const;
    unsigned long long get_id() const;


private:
    std::string title;
    std::string author;
    std::string text;
    unsigned long long id;
};

#endif