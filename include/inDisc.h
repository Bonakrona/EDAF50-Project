#ifndef INDISC_H
#define INDISC_H

#include <string>
#include <vector>
#include <unordered_map>
#include "article.h"
#include "newsgroup.h"
#include "database.h"
#include <filesystem>

namespace fs = std::filesystem;
using std::string;

class inDisc: public Database
{
public:
    inDisc();
    std::vector<Newsgroup> listNewsgroups() override;
    bool addNewsgroup(const string n) override;
    bool removeNewsgroup(unsigned long long id) override;
    std::optional<std::reference_wrapper<Newsgroup>> getNewsgroup(unsigned long long id) override;

    std::vector<Article> listNewsgroupsArticles(Newsgroup &ng) override;
    bool addNewsgroupsArticle(const std::string &t, const std::string &a, const std::string &txt, Newsgroup &ng) override;
    bool removeNewsgroupsArticle(unsigned long long id, Newsgroup &ng) override;
    std::optional<Article> getNewsgroupsArticle(unsigned long long id, Newsgroup &ng) const override;

private:
    std::map<unsigned long long, Newsgroup> newsgroups;
    std::unordered_set<std::string> newsgroupNames;
    unsigned long long nextNewsgroupID; // technically maximum here
    fs::path root = fs::current_path() / "Database";
    std::map<unsigned long long, fs::path> newsgroupPaths;
    
    void loadDisc();
    std::vector<fs::path> findDirectories(fs::path currentPath);
    std::vector<fs::path> findFiles(fs::path currentPath);
};
#endif