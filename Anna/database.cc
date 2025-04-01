#include "database.h"

Database::Database(){
    nextNewsgroupID = 1;
};

bool Database::addNewsgroup(const std::string n){
    if(newsgroupNames.find(n) != newsgroupNames.end()){
        return false;
    }else{
        newsgroups[nextNewsgroupID] = Newsgroup(n);
        newsgroupNames.insert(n);
        nextNewsgroupID++;
        return true;
    }
};

std::optional<Newsgroup> Database::getNewsgroup(unsigned long long id) const{
    auto it = newsgroups.find(id);
    if(it != newsgroups.end()){
        return it->second;
    }else{
        return std::nullopt;
    }
};

bool Database::removeNewsgroup(unsigned long long id){
    auto it = newsgroups.find(id);
    if (it != newsgroups.end()) {
        newsgroupNames.erase(it->second.get_name());
        newsgroups.erase(it);
        return true;
    }
    return false;
};

void Database::listNewsgroups() const{
    if (newsgroups.empty()) {
        std::cout << "No newsgroups available.\n";
        return;
    }

    for (const auto& pair : newsgroups) {
        std::cout << "ID: " << pair.first << "; Name: " << pair.second.get_name() << "\n";
    }
}