#include "inMemory.h"
#include "inDisc.h"
#include "database.h"
#include "newsgroup.h"
#include "article.h"

#include <fstream>
#include <iostream>
#include <cassert>

void testArticle() {
    Article article("News1", "Max Mustermann", "These are the news.", 1);
    assert(article.get_title() == "News1");
    assert(article.get_author() == "Max Mustermann");
    assert(article.get_content() == "These are the news.");
    std::cout << "Article tests passed!" << std::endl;
}

void testNewsgroup() {
    Newsgroup group("Fun News", 1);

    assert(group.get_name() == "Fun News");

    bool added1 = group.addArticle("News1", "Max Mustermann", "These are the news.");
    assert(added1 == true);

    auto A = group.getArticle(1);
    assert(A.has_value() && A->get_title() == "News1");
    
    bool not_added = group.addArticle("News1", "Maria Mustermann", "These are more news!");
    assert(not_added == false); // has same title as previous article, should not be added
    
    bool added2 = group.addArticle("News2", "Maria Mustermann", "These are more news!");
    assert(added2 == true);
    
    Article a1("News1", "Max Mustermann", "These are the news.", 1);
    Article a2("News2", "Maria Mustermann", "These are more news!", 2);
    
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

void testInMemory(){
    inMemory db;

    bool added1 = db.addNewsgroup("New News");
    bool not_added = db.addNewsgroup("New News");
    bool added2 = db.addNewsgroup("Not News");

    assert(added1 == true);
    assert(not_added == false);
    assert(added2 == true);

    auto N = db.getNewsgroup(1);
    assert(N.has_value());
    assert(N->get().get_name() == "New News");
    bool removed = db.removeNewsgroup(1);
    assert(removed == true);
    assert(db.getNewsgroup(1) == std::nullopt);
    assert(db.removeNewsgroup(3) == false);

    Article art1("News1", "Max Mustermann", "These are the news.", 1);
    Article art2("News2", "Anna Kristina", "Bla bla bla.", 2);
    Article art3("News3", "Jona Anhalte", "Important news.", 3);

    std::vector<Article> exampleArticleVector {art1, art2, art3};
    
    bool addedA1 = db.addNewsgroupsArticle("News1", "Max Mustermann", "These are the news.", N.value());
    bool addedA2 = db.addNewsgroupsArticle("News2", "Anna Kristina", "Bla bla bla.", N.value());
    bool addedA3 = db.addNewsgroupsArticle("News3", "Jona Anhalte", "Important news.", N.value());
    assert(addedA1 == true);
    assert(addedA2 == true);
    assert(addedA3 == true);
    
    //befor removal
    std::vector<Article> listedNewsgroupArticles = db.listNewsgroupsArticles(N.value());
    
    assert(listedNewsgroupArticles.size() == exampleArticleVector.size());

    for (size_t i = 0; i < exampleArticleVector.size(); ++i) {
        assert(listedNewsgroupArticles[i].get_title() == exampleArticleVector[i].get_title());
        assert(listedNewsgroupArticles[i].get_author() == exampleArticleVector[i].get_author());
        assert(listedNewsgroupArticles[i].get_content() == exampleArticleVector[i].get_content());
        assert(listedNewsgroupArticles[i].get_id() == exampleArticleVector[i].get_id());
    }

    // removal
    bool removedNewsgroupArticle = db.removeNewsgroupsArticle(1, N.value());
    assert(removedNewsgroupArticle == true);

    auto retrievedNewsgroupArticle1 = db.getNewsgroupsArticle(1, N.value());
    auto retrievedNewsgroupArticle2 = db.getNewsgroupsArticle(2, N.value());

    assert(retrievedNewsgroupArticle1 == std::nullopt);
    assert(retrievedNewsgroupArticle2.has_value());

    //after removal
    std::vector<Article> exampleArticleVector2 {art2, art3};
    std::vector<Article> listedNewsgroupArticles2 = db.listNewsgroupsArticles(N.value());
    
    assert(listedNewsgroupArticles2.size() == exampleArticleVector2.size());

    for (size_t i = 0; i < exampleArticleVector2.size(); ++i) {
        assert(listedNewsgroupArticles2[i].get_title() == exampleArticleVector2[i].get_title());
        assert(listedNewsgroupArticles2[i].get_author() == exampleArticleVector2[i].get_author());
        assert(listedNewsgroupArticles2[i].get_content() == exampleArticleVector2[i].get_content());
        assert(listedNewsgroupArticles2[i].get_id() == exampleArticleVector2[i].get_id());
    }

    std::cout << "InMemory database tests passed!" << std::endl;
}

void testInDisc(){

    fs::remove_all("Database");

    inDisc db;

    bool added1 = db.addNewsgroup("New News");
    bool not_added = db.addNewsgroup("New News");
    bool added2 = db.addNewsgroup("Not News");

    assert(added1 == true);
    assert(not_added == false);
    assert(added2 == true);

    auto groups = db.listNewsgroups();
    assert(groups.size() == 2);
    auto& ng = groups[0];

    assert(db.addNewsgroupsArticle("News1", "Max Mustermann", "These are the news.", ng) == true);

    fs::path root = fs::current_path() / "Database";
    fs::path metaFile = root / "metadata.txt";
    assert(fs::exists(metaFile));
    std::ifstream inMeta(metaFile);
    int storedNextId;
    inMeta >> storedNextId;
    assert(storedNextId == 3);

    fs::path groupDir1 = root / "1_New News";
    assert(fs::exists(groupDir1));

    fs::path groupDir2 = root / "2_Not News";
    assert(fs::exists(groupDir2));

    fs::path groupMeta = groupDir1 / "metadata.txt";
    assert(fs::exists(groupMeta));
    std::ifstream inGroupMeta(groupMeta);
    int nextArticleID;
    inGroupMeta >> nextArticleID;
    assert(nextArticleID == 2);

    fs::path articleFile = groupDir1 / "articles" / "1.txt";
    assert(fs::exists(articleFile));
    std::ifstream inArticle(articleFile);
    std::string line;
    std::getline(inArticle, line);
    assert(line == "1");
    std::getline(inArticle, line);
    assert(line == "News1");
    std::getline(inArticle, line);
    assert(line == "Max Mustermann");
    std::getline(inArticle, line);
    assert(line == "These are the news.");

    bool removed = db.removeNewsgroupsArticle(1, ng);
    assert(removed == true);
    assert(!fs::exists(articleFile));

    bool removed2 = db.removeNewsgroup(ng.get_id());
    assert(removed2 == true);
    assert(!fs::exists(groupDir1));

    std::cout << "InDisc database tests passed!" << std::endl;
}

int main() {
    testArticle();
    testNewsgroup();
    testInMemory();
    testInDisc();
    std::cout << "All tests passed successfully!" << std::endl;
    return 0;
}