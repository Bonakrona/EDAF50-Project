#include "article.h"

Article::Article(const std::string& t, const std::string& a, const std::string& txt, const unsigned long long i){
    title = t;
    author = a;
    text = txt;
    id = i;
};

std::string Article::get_author() const{
    return author;
};

std::string Article::get_title() const{
    return title;
};

std::string Article::get_content() const{
    return text;
}

unsigned long long Article::get_id() const{
    return id;
}