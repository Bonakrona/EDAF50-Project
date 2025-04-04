#ifndef ARTICLE_H
#define ARTICLE_H

#include <iostream>
#include <vector>
#include <string>
#include "database.h"

class Article {

public:
    Article() = default;
    Article(const std::string& t, const std::string& a, const std::string& txt, unsigned long long id);
    std::string get_author() const; //slower than making author and title simply public
    std::string get_title() const;
    std::string get_content() const;
    unsigned long long Article::get_id() const;


private:
    std::string title;
    std::string author;
    std::string text;
    unsigned long long id;
};

#endif