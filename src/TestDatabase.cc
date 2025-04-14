#include "inMemory.h"
#include "database.h"
#include "newsgroup.h"
#include "article.h"

#include <iostream>
#include <cassert>

void test_article() {
    Article article("News1", "Max Mustermann", "These are the news.", 1);
    assert(article.get_title() == "News1");
    assert(article.get_author() == "Max Mustermann");
    assert(article.get_content() == "These are the news.");
    std::cout << "Article tests passed!" << std::endl;
}

void test_newsgroup() {
    Newsgroup group("Fun News");

    assert(group.get_name() == "Fun News");

    bool added1 = group.addArticle("News1", "Max Mustermann", "These are the news.");
    Article a1("News1", "Max Mustermann", "These are the news.", 2);
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
    inMemory test;

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

    Article art1("News1", "Max Mustermann", "These are the news.", 1);
    Article art2("News2", "Anna Kristina", "Bla bla bla.", 2);
    Article art3("News3", "Jona Anhalte", "Important news.", 3);

    std::vector<Article> exampleArticleVector {art1, art2, art3};
    
    bool addedA1 = test.addNewsgroupsArticle("News1", "Max Mustermann", "These are the news.", n1);
    bool addedA2 = test.addNewsgroupsArticle("News2", "Anna Kristina", "Bla bla bla.", n1);
    bool addedA3 = test.addNewsgroupsArticle("News3", "Jona Anhalte", "Important news.", n1);
    assert(addedA1 == true);
    assert(addedA2 == true);
    assert(addedA3 == true);
    
    std::vector<Article> listedNewsgroupArticles = test.ListNewsgroupsArticles(n1);
    assert(listedNewsgroupArticles == exampleArticleVector);



    bool removedNewsgroupArticle = test.removeNewsgroupsArticle(1, n1);
    assert(removedNewsgroupArticle == true);

    auto retrievedNewsgroupArticle1 = test.getNewsgroupsArticle(1, n1);
    auto retrievedNewsgroupArticle2 = test.getNewsgroupsArticle(2, n1);

    assert(retrievedNewsgroupArticle1 == std::nullopt);
    assert(retrievedNewsgroupArticle2.has_value());

    std::cout << "Database tests passed!" << std::endl;
}

int main() {
    test_article();
    test_newsgroup();
    test_database();
    std::cout << "All tests passed successfully!" << std::endl;
    return 0;
}