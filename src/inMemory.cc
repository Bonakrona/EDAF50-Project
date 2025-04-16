#include "inMemory.h"


inMemory::inMemory(){
    nextNewsgroupID = 1;
};

bool inMemory::addNewsgroup(const std::string n){
    if(newsgroupNames.find(n) != newsgroupNames.end()){
        return false;
    }else{
        newsgroups[nextNewsgroupID] = Newsgroup(n, nextNewsgroupID);
        newsgroupNames.insert(n);
        nextNewsgroupID++;
        return true;
    }
};

std::optional<Newsgroup> inMemory::getNewsgroup(unsigned long long id) const{
    auto it = newsgroups.find(id);
    if(it != newsgroups.end()){
        return it->second;
    }else{
        return std::nullopt;
    }
};

bool inMemory::removeNewsgroup(unsigned long long id) {
    auto it = newsgroups.find(id);
    if (it != newsgroups.end()) {
        newsgroupNames.erase(it->second.get_name());
        newsgroups.erase(it);
        return true;
    }
    return false;
};

std::vector<Newsgroup> inMemory::listNewsgroups() {
    std::vector<Newsgroup> res;
    for (const auto &pair: newsgroups) {
        res.push_back(pair.second);
    }
    return res;
}

std::vector<Article> inMemory::listNewsgroupsArticles(Newsgroup &ng) {
    return ng.listArticles();
}

bool inMemory::addNewsgroupsArticle(const std::string &t, const std::string &a, const std::string &txt, Newsgroup &ng) {
    return ng.addArticle(t, a, txt);
}

bool inMemory::removeNewsgroupsArticle(unsigned long long id, Newsgroup &ng) {
    return ng.removeArticle(id);
}

std::optional<Article> inMemory::getNewsgroupsArticle(unsigned long long id, Newsgroup &ng) const{
    return ng.getArticle(id);
}