#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include<map>
#include<string>
#include<optional>
#include "newsgroup.h"

// ANNA
//do we need this? 
//not sure if I am missing something but that seemed the most logical option to me

class Database {

public:
    //constructor
    // should be called automatically when programme is started for the in-memory server
    // gets only called once for the disk server
    Database();

    bool addNewsgroup(const std::string n);
    std::optional<Newsgroup> getNewsgroup(unsigned long long id) const;
    bool removeNewsgroup(unsigned long long id);
    void listNewsgroups() const;

private:
    std::map<unsigned long long, Newsgroup> newsgroups;
    std::unordered_set<std::string> newsgroupNames;
    unsigned long long nextNewsgroupID; //technically maximum here

};

#endif