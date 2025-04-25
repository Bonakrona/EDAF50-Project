#include "InDisc.h"
#include <iostream>
#include <fstream>


InDisc::InDisc(){
    fs::path metadataPath = root / "metadata.txt";
    if(fs::exists(metadataPath)) {
        std::ifstream in(metadataPath);
        in >> nextNewsgroupID;
    } else {
        nextNewsgroupID = 1;
        std::ofstream(metadataPath);
        metadataPath << nextNewsgroupID;
    }
};

bool InDisc::addNewsgroup(const std::string n){
    if(newsgroupNames.find(n) != newsgroupNames.end()){
        return false;
    }else{
        newsgroups[nextNewsgroupID] = Newsgroup(n, nextNewsgroupID);
        newsgroupNames.insert(n);
        nextNewsgroupID++;
        return true;
    }
};

std::optional<std::reference_wrapper<Newsgroup>> InDisc::getNewsgroup(unsigned long long id) {
    auto it = newsgroups.find(id);
    if(it != newsgroups.end()){
        return std::ref(it->second);
    }else{
        return std::nullopt;
    }
};

bool InDisc::removeNewsgroup(unsigned long long id) {
    auto it = newsgroups.find(id);
    if (it != newsgroups.end()) {
        newsgroupNames.erase(it->second.get_name());
        newsgroups.erase(it);
        return true;
    }
    return false;
};

std::vector<Newsgroup> InDisc::listNewsgroups() {
    std::vector<Newsgroup> res;
    for (const auto &pair: newsgroups) {
        res.push_back(pair.second);
    }
    return res;
}

std::vector<Article> InDisc::listNewsgroupsArticles(Newsgroup &ng) {
    return ng.listArticles();
}

bool InDisc::addNewsgroupsArticle(const std::string &t, const std::string &a, const std::string &txt, Newsgroup &ng) {
    return ng.addArticle(t, a, txt);
}

bool InDisc::removeNewsgroupsArticle(unsigned long long id, Newsgroup &ng) {
    return ng.removeArticle(id);
}

std::optional<Article> InDisc::getNewsgroupsArticle(unsigned long long id, Newsgroup &ng) const{
    return ng.getArticle(id);
}