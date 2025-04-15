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
    assert(added1 == true);

    Article a1("News1", "Max Mustermann", "These are the news.", 1);
    auto A = group.getArticle(1);
    assert(A.has_value() && A->get_title() == a1.get_title());
    
    bool not_added = group.addArticle("News1", "Maria Mustermann", "These are more news!");
    assert(not_added == false); // has same title as previous article, should not be added

    bool added2 = group.addArticle("News2", "Maria Mustermann", "These are more news!");
    Article a2("News2", "Maria Mustermann", "These are more news!", 2);
    assert(added2 == true);

    //before removal
    std::vector<Article> expected = {a1, a2};
    std::vector<Article> listed = group.listArticles();

    assert(listed.size() == expected.size());

    for (size_t i = 0; i < expected.size(); ++i) {
        assert(listed[i].get_title() == expected[i].get_title());
        assert(listed[i].get_author() == expected[i].get_author());
        assert(listed[i].get_content() == expected[i].get_content());
        assert(listed[i].get_id() == expected[i].get_id());
    }
    
    //removal
    assert(group.removeArticle(1) == true);
    assert(group.getArticle(1) == std::nullopt);
    assert(group.removeArticle(3) == false);

    //after removal
    std::vector<Article> expected2 = {a2};
    std::vector<Article> listed2 = group.listArticles();

    assert(listed2.size() == expected2.size());

    for (size_t i = 0; i < expected2.size(); ++i) {
        assert(listed2[i].get_title() == expected2[i].get_title());
        assert(listed2[i].get_author() == expected2[i].get_author());
        assert(listed2[i].get_content() == expected2[i].get_content());
        assert(listed2[i].get_id() == expected2[i].get_id());
    }
    
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

    auto N = test.getNewsgroup(1);
    assert(N.has_value());
    assert(N->get_name() == n1.get_name());
    bool removed = test.removeNewsgroup(1);
    assert(removed == true);
    assert(test.getNewsgroup(1) == std::nullopt);
    assert(test.removeNewsgroup(3) == false);

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
    
    //befor removal
    std::vector<Article> listedNewsgroupArticles = test.listNewsgroupsArticles(n1);
    
    assert(listedNewsgroupArticles.size() == exampleArticleVector.size());

    for (size_t i = 0; i < exampleArticleVector.size(); ++i) {
        assert(listedNewsgroupArticles[i].get_title() == exampleArticleVector[i].get_title());
        assert(listedNewsgroupArticles[i].get_author() == exampleArticleVector[i].get_author());
        assert(listedNewsgroupArticles[i].get_content() == exampleArticleVector[i].get_content());
        assert(listedNewsgroupArticles[i].get_id() == exampleArticleVector[i].get_id());
    }

    // removal
    bool removedNewsgroupArticle = test.removeNewsgroupsArticle(1, n1);
    assert(removedNewsgroupArticle == true);

    auto retrievedNewsgroupArticle1 = test.getNewsgroupsArticle(1, n1);
    auto retrievedNewsgroupArticle2 = test.getNewsgroupsArticle(2, n1);

    assert(retrievedNewsgroupArticle1 == std::nullopt);
    assert(retrievedNewsgroupArticle2.has_value());

    //after removal
    std::vector<Article> exampleArticleVector2 {art2, art3};
    std::vector<Article> listedNewsgroupArticles2 = test.listNewsgroupsArticles(n1);
    
    assert(listedNewsgroupArticles2.size() == exampleArticleVector2.size());

    for (size_t i = 0; i < exampleArticleVector2.size(); ++i) {
        assert(listedNewsgroupArticles2[i].get_title() == exampleArticleVector2[i].get_title());
        assert(listedNewsgroupArticles2[i].get_author() == exampleArticleVector2[i].get_author());
        assert(listedNewsgroupArticles2[i].get_content() == exampleArticleVector2[i].get_content());
        assert(listedNewsgroupArticles2[i].get_id() == exampleArticleVector2[i].get_id());
    }

    std::cout << "Database tests passed!" << std::endl;
}

int main() {
    test_article();
    test_newsgroup();
    test_database();
    std::cout << "All tests passed successfully!" << std::endl;
    return 0;
}