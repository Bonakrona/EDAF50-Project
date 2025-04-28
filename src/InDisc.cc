#include <fstream>
#include "InDisc.h"
#include <iostream>
#include <sstream>

InDisc::InDisc()
{
    fs::path metadataPath = root / "metadata.txt";
    if (fs::exists(metadataPath))
    {
        std::ifstream in(metadataPath);
        in >> nextNewsgroupID;

        std::vector<fs::path> directory = findDirectories(root);
        if (!directory.empty())
        {
            for (auto &file : directory)
            {
                std::string strFile = file.string();
                std::string s;
                std::stringstream strs(strFile);

                unsigned long long id;

                while (getline(strs, s, '_'))
                {
                    id = std::stoull(s.substr(0, s.find('_')));
                    std::string name = s.substr(s.find('_') + 1);

                    fs::path currentMetadata = file / "metadata.txt";
                    unsigned long long newsgroupNextArticleId;
                    if (fs::exists(currentMetadata))
                    {
                        std::ifstream in(currentMetadata);
                        in >> newsgroupNextArticleId;
                    }
                    newsgroups[id] = Newsgroup(name, id, newsgroupNextArticleId);
                    newsgroupNames.insert(name);
                    newsgroupPaths[id] = strFile;
                }
                std::vector<fs::path> articleFiles = findFiles(file / "articles");
                if (!articleFiles.empty())
                {
                    for (auto &articleFile : articleFiles)
                    {
                        std::ifstream artFile(articleFile);
                        
                        while (getline(artFile, s, '\n'))
                        {
                            unsigned long long articleId = std::stoull(s.substr(0, s.find('\n')));
                            std::string title = s.substr(s.find('\n') + 1);
                            std::string author = s.substr(s.find('\n') + 1);
                            std::string text = s.substr(s.find('\n') + 1);
                            newsgroups[id].addArticle(title, author, text, articleId);
                        }
                    }
                }
            }
        }
        else
        {
            nextNewsgroupID = 1;
            std::ofstream(metadataPath);
            metadataPath << nextNewsgroupID;
        }
    }
}

std::vector<fs::path> findDirectories(fs::path currentPath)
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

std::vector<fs::path> findFiles(fs::path currentPath)
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

bool InDisc::addNewsgroup(const std::string n)
{
    fs::path groupDir = root / (std::to_string(nextNewsgroupID) + "_" + n);
    fs::create_directories(groupDir);

    if (newsgroupNames.find(n) != newsgroupNames.end())
    {
        return false;
    }
    else
    {
        newsgroups[nextNewsgroupID] = Newsgroup(n, nextNewsgroupID);
        newsgroupNames.insert(n);
        newsgroupPaths[nextNewsgroupID] = groupDir;
        nextNewsgroupID++;
        return true;
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
    return true;
};

std::optional<std::reference_wrapper<Newsgroup>> InDisc::getNewsgroup(unsigned long long id)
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

bool InDisc::removeNewsgroup(unsigned long long id)
{
    auto it = newsgroups.find(id);
    if (it != newsgroups.end())
    {
        newsgroupNames.erase(it->second.get_name());
        newsgroups.erase(it);

        fs::path articlePath = newsgroupPaths[id];
        fs::remove(articlePath);
        
        newsgroupPaths.erase(id);

        return true;
    }
    return false;
};

std::vector<Newsgroup> InDisc::listNewsgroups()
{
    std::vector<Newsgroup> res;
    for (const auto &pair : newsgroups)
    {
        res.push_back(pair.second);
    }
    return res;
}

std::vector<Article> InDisc::listNewsgroupsArticles(Newsgroup &ng)
{
    return ng.listArticles();
}

bool InDisc::addNewsgroupsArticle(const std::string &t, const std::string &a, const std::string &txt, Newsgroup &ng)
{
    fs::path newsgroupPath = newsgroupPaths[ng.get_id()];
    std::ofstream metadataFile(newsgroupPath / "metadata.txt");
    unsigned long long tempID = ng.nextArcticleID + 1;
    if (metadataFile.is_open())
    {
        metadataFile << tempID;
        metadataFile.close();

        std::ofstream articleFile(newsgroupPath / "article" / (std::to_string(tempID) + ".txt"));
        if (articleFile.is_open())
        {
            articleFile << tempID << '\n' << t << '\n' << a << '\n' << txt;
            articleFile.close();
        }
    }
    return ng.addArticle(t, a, txt);
}

bool InDisc::removeNewsgroupsArticle(unsigned long long id, Newsgroup &ng)
{
    fs::path articlePath = newsgroupPaths[ng.get_id()] / "article" / (std::to_string(id) + ".txt");
    fs::remove(articlePath);
    return ng.removeArticle(id);
}

std::optional<Article> InDisc::getNewsgroupsArticle(unsigned long long id, Newsgroup &ng) const
{
    return ng.getArticle(id);
}