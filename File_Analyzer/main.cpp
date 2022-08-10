#include<iostream>
#include"interface.h"
#include<string>
#include<fstream>
#include<sstream>
#include<exception>
#include<dirent.h>
#include<sys/stat.h>
#include<errno.h>

// check if entered path is correct 
// if its a directory -> message + return false
bool correctFilePath(std::string& path) 
{
    if(path.empty())
    {
        throw std::invalid_argument("empty path");
    }
    char* ptr = &path[0];
    struct stat s;
    if(stat(ptr , &s) == 0)
    {
        if(s.st_mode & S_IFREG) // its a file
        {
            return true;
        }
        else if(s.st_mode & S_IFDIR) // its a directory
        {
            std::cout<<"its a directory"<<std::endl;
            return false;
        }
        else // its something else
        {
            return false;
        }
    }
    else // error
    {
        throw std::invalid_argument("error in the path");
    }

}

void enterPath(std::string& path)
{
    bool correctPath = false;
    do
    {
        std::cin >> path;
        try
        {
            correctPath = correctFilePath(path);
        }
        catch(...)
        {
            correctPath = false;
            std::cout<<"error in path"<<std::endl;
        }
        if (!correctPath)
        {
            std::cout << "not correct path , enter again" << std::endl;
        }

    } while (!correctPath);
}

void clearEscapeSequence(std::string& path)
{
    int len = path.length();
    for(int i= 0 ; i < len ; )
    {
        if(path[i] == 92 && path[i+1] != 92)
        {
            path.erase(i , 2);
            continue;
        }
        ++i;
    }
}

std::string getStream(const std::string& path)
{
    std::ifstream fileStream(path);
    if(fileStream.good())
    {
        std::string words , currentWord;
        while(!fileStream.eof())
        {
            
            fileStream>>currentWord;
            clearEscapeSequence(currentWord); // erase escape sequene from the word
            if(currentWord.empty())
            {
                continue;
            }
            words += (currentWord + " ");
            currentWord = "";
        }
        words.pop_back();
        return words;
    }
    else
    {
        throw std::invalid_argument("can't open the file");
    }
    fileStream.close();
}



void outputMultiset(WordsMultiset& multiset , const std::string message)
{
    std::multiset<std::string> multis(multiset.words());
    int size = multis.size();
    std::cout<<size<<" " <<message<<std::endl;
    for( std::multiset<std::string>::iterator it = multis.begin();  it != multis.end() ; ++it)
    {
        std::cout<<*it<<" ";
    }
     std::cout<<'\n'<<std::endl;
}

void start(std::string& firstPath , std::string& secondPath)
{
    const std::string fisrtStream = getStream(firstPath);
    const std::string secondStream = getStream(secondPath);

    std::stringstream first(fisrtStream);
    std::stringstream second(secondStream);

    Comparator cmp;
    ComparisonReport report = cmp.compare(first, second);
    std::cout<<std::endl;

    outputMultiset(report.commonWords , "common words");
    outputMultiset(report.uniqueWords[0] , "unique words in first file");
    outputMultiset(report.uniqueWords[1] , "unique words in second file");

}

int main()
{
    std::string firstFileName,secondFileName;
    enterPath(firstFileName);
    enterPath(secondFileName);
    start(firstFileName , secondFileName);

    return 0;
}