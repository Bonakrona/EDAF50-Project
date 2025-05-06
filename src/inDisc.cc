#include <fstream>
#include "inDisc.h"
#include <iostream>
#include <sstream>

inDisc::inDisc()
{
    fs::path metadataPath = root / "metadata.txt";
    if (fs::exists(root) && fs::exists(metadataPath))
    {
        loadFromDisc();
    }
    else if(fs::exists(root) && !fs::exists(metadataPath))
    {
        fs::remove_all(root);
        createOnDisc();
    }
    else
    {
        createOnDisc();
    }
}

void inDisc::loadFromDisc(){
    fs::path metadataPath = root / "metadata.txt";
    std::ifstream in(metadataPath);
    in >> nextNewsgroupID;

    std::vector<fs::path> newsgroupDirectories = findDirectories(root);
    if (!newsgroupDirectories.empty()) {
        for (auto &newsgroupDirectory : newsgroupDirectories) {
            std::string directoryName = newsgroupDirectory.filename().string();
            size_t underscorePos = directoryName.find('_');

            if (underscorePos == std::string::npos) {
                continue;
            }

            std::string idStr = directoryName.substr(0, underscorePos);
            std::string name = directoryName.substr(underscorePos + 1);
            unsigned long long id;

            try {
                id = std::stoull(idStr);
            } catch (const std::exception& e) {
                continue;
            }

            fs::path directoryMetadata = newsgroupDirectory / "metadata.txt";
            unsigned long long newsgroupNextArticleID = 1;
            if (fs::exists(directoryMetadata)) {
                std::ifstream in(directoryMetadata);
                in >> newsgroupNextArticleID;
            } else {
                fs::remove_all(newsgroupDirectory);
                continue;
            }
            newsgroups[id] = Newsgroup(name, id, newsgroupNextArticleID);
            newsgroupNames.insert(name);
            newsgroupPaths[id] = newsgroupDirectory.string();

            if (!fs::exists(newsgroupDirectory / "articles")) {
                fs::create_directories(newsgroupDirectory / "articles");
            }
            
            std::vector<fs::path> articleFiles = findFiles(newsgroupDirectory / "articles");
            if (!articleFiles.empty()) {
                for (auto &articleFile : articleFiles) {
                    std::ifstream artFile(articleFile);
                    std::string s;
                    unsigned long long articleId;
                    std::string title, author, text;

                    if (std::getline(artFile, s)) {
                        try {
                            articleId = std::stoull(s);
                            std::getline(artFile, title);
                            std::getline(artFile, author);
                            std::getline(artFile, text);
                            newsgroups[id].addArticle(title, author, text, articleId);
                        } catch (const std::exception& e) {
                            continue;
                        }
                    }
                }
            }
        }
    }
}

void inDisc::createOnDisc(){
    fs::create_directories("Database");
    fs::path metadataPath = root / "metadata.txt";
    nextNewsgroupID = 1;
    std::ofstream metadata(metadataPath);
    metadata << nextNewsgroupID;
}

std::vector<fs::path> inDisc::findDirectories(fs::path currentPath)
{
    std::vector<fs::path> directories;
    for (auto &p : fs::recursive_directory_iterator(currentPath))
    {
        if (p.is_directory())
        {
            directories.push_back(p.path());
        }
    }
    return directories;
}

std::vector<fs::path> inDisc::findFiles(fs::path currentPath)
{
    std::vector<fs::path> files;
    for (auto &p : fs::recursive_directory_iterator(currentPath))
    {
        if (p.is_regular_file())
        {
            files.push_back(p.path());
        }
    }
    return files;
}

bool inDisc::addNewsgroup(const std::string n)
{
    fs::path groupDir = root / (std::to_string(nextNewsgroupID) + "_" + n);
    
    if (newsgroupNames.find(n) != newsgroupNames.end())
    {
        return false;
    }
    else
    {
        fs::create_directories(groupDir);
        
        newsgroups[nextNewsgroupID] = Newsgroup(n, nextNewsgroupID);
        newsgroupNames.insert(n);
        newsgroupPaths[nextNewsgroupID] = groupDir;
        nextNewsgroupID++;
    }
    
    std::ofstream newsgroupMetadata(groupDir / "metadata.txt");
    if (newsgroupMetadata.is_open())
    {
        newsgroupMetadata << "1";
        newsgroupMetadata.close();
    }
    
    std::ofstream dbMetadataFile(root / "metadata.txt");
    if (dbMetadataFile.is_open())
    {
        dbMetadataFile << nextNewsgroupID;
        dbMetadataFile.close();
    }

    fs::create_directories(groupDir/ "articles");

    return true;
};

std::optional<std::reference_wrapper<Newsgroup>> inDisc::getNewsgroup(unsigned long long id)
{
    auto it = newsgroups.find(id);
    if (it != newsgroups.end())
    {
        return std::ref(it->second);
    }
    else
    {
        return std::nullopt;
    }
};

bool inDisc::removeNewsgroup(unsigned long long id)
{
    auto it = newsgroups.find(id);
    if (it != newsgroups.end())
    {
        newsgroupNames.erase(it->second.get_name());
        newsgroups.erase(it);

        fs::path articlePath = newsgroupPaths[id];
        fs::remove_all(articlePath);
        
        newsgroupPaths.erase(id);

        return true;
    }
    return false;
};

std::vector<Newsgroup> inDisc::listNewsgroups()
{
    std::vector<Newsgroup> res;
    for (const auto &pair : newsgroups)
    {
        res.push_back(pair.second);
    }
    return res;
}

std::vector<Article> inDisc::listNewsgroupsArticles(Newsgroup &ng)
{
    return ng.listArticles();
}

bool inDisc::addNewsgroupsArticle(const std::string &t, const std::string &a, const std::string &txt, Newsgroup &ng)
{
    fs::path newsgroupPath = newsgroupPaths[ng.get_id()];
    std::ofstream metadataFile(newsgroupPath / "metadata.txt");
    unsigned long long tempID = ng.nextArcticleID + 1;
    if (metadataFile.is_open())
    {
        metadataFile << tempID;
        metadataFile.close();

        std::ofstream articleFile(newsgroupPath / "articles" / (std::to_string(ng.nextArcticleID) + ".txt"));
        if (articleFile.is_open())
        {
            articleFile << ng.nextArcticleID << '\n' << t << '\n' << a << '\n' << txt;
            articleFile.close();
        }
    }
    return ng.addArticle(t, a, txt);
}

bool inDisc::removeNewsgroupsArticle(unsigned long long id, Newsgroup &ng)
{
    fs::path articlePath = newsgroupPaths[ng.get_id()] / "articles" / (std::to_string(id) + ".txt");
    fs::remove(articlePath);
    return ng.removeArticle(id);
}

std::optional<Article> inDisc::getNewsgroupsArticle(unsigned long long id, Newsgroup &ng) const
{
    return ng.getArticle(id);
}