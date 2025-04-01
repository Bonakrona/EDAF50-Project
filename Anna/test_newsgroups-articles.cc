#include "database.h"
#include "newsgroup.h"
#include "article.h"

#include <iostream>
#include <cassert>

void test_article() {
    Article article("News1", "Max Mustermann", "These are the news.");
    assert(article.get_title() == "News1");
    assert(article.get_author() == "Max Mustermann");
    assert(article.get_content() == "These are the news.");
    std::cout << "Article tests passed!" << std::endl;
}

void test_newsgroup() {
    Newsgroup group("Fun News");

    assert(group.get_name() == "Fun News");

    bool added1 = group.addArticle("News1", "Max Mustermann", "These are the news.");
    Article a1("News1", "Max Mustermann", "These are the news.");
    bool not_added = group.addArticle("News1", "Maria Mustermann", "These are more news!");
    bool added2 = group.addArticle("News2", "Maria Mustermann", "These are more news!");

    assert(added1 == true);
    assert(not_added == false);
    assert(added2 == true);

    std::cout << "Before removal:" << std::endl;
    group.listArticles();
    
    auto A = group.getArticle(1);
    assert(A.has_value());
    assert(A->get_title() == a1.get_title());
    assert(group.removeArticle(1) == true);
    assert(group.getArticle(1) == std::nullopt);
    assert(group.removeArticle(3) == false);
    
    std::cout << "After removal:" << std::endl;
    group.listArticles();
    
    std::cout << "NewsGroup tests passed!" << std::endl;
}

void test_database(){
    Database test;

    bool added1 = test.addNewsgroup("New News");
    Newsgroup n1("New News");
    bool not_added = test.addNewsgroup("New News");
    bool added2 = test.addNewsgroup("Not News");

    assert(added1 == true);
    assert(not_added == false);
    assert(added2 == true);

    std::cout << "Before removal: \n";
    test.listNewsgroups();

    auto N = test.getNewsgroup(1);
    assert(N.has_value());
    assert(N->get_name() == n1.get_name());
    bool removed = test.removeNewsgroup(1);
    assert(removed == true);
    assert(test.getNewsgroup(1) == std::nullopt);
    assert(test.removeNewsgroup(3) == false);

    std::cout << "After removal: \n";
    test.listNewsgroups();

    std::cout << "Database tests passed!" << std::endl;
}

int main() {
    test_article();
    test_newsgroup();
    test_database();
    std::cout << "All tests passed successfully!" << std::endl;
    return 0;
}
