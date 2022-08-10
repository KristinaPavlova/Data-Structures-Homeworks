#ifndef INTERFACE_CPP
#define INTERFACE_CPP
#include"interface.h"


WordsMultiset::WordsMultiset (const WordsMultiset& other)
{
    if(!other.pairs.empty())
    {
        std::vector<pair<std::string , int>> otherPairs = other.pairs.inorder();
        int size = other.pairs.getSize();
        for(int i = 0 ; i < size ; ++i)
        {
            this->pairs.insert(otherPairs[i]);
        }
    }
}

void WordsMultiset::add(const std::string& word, size_t times )
{
    pair<std::string , int> currentWord(word , times);
    node<pair<std::string , int>>* newWord = pairs.find(currentWord);
    if(newWord != nullptr) //  word already exist increase number
    {
        newWord->value.getSecond() += times;
    }
    else // word doesn't exist add new word to the tree
    {
        pairs.insert(currentWord);
    }
}

bool WordsMultiset::contains(const std::string& word) const
{
    pair<std::string , int> currentWord(word , 1);
    node<pair<std::string , int>>* newWord = pairs.find(currentWord);
    if(newWord != nullptr)
    {
       return true;
    }
    return false;
}

size_t WordsMultiset::countOf(const std::string& word) const
{
    pair<std::string , int> currentWord(word , 1);
    node<pair<std::string , int>>* newWord = pairs.find(currentWord);
    if(newWord != nullptr)
    {
       return newWord->value.getSecond();
    }
    return 0;

}

size_t WordsMultiset::countOfUniqueWords() const
{
    return pairs.getSize();
}

std::multiset<std::string> WordsMultiset::words() const
{
    std::vector<pair<std::string , int>> wordsPair = pairs.inorder();
    int size = wordsPair.size();
    std::multiset<std::string> addedWords;
    int wordsOccurences = 0;
    for(int i = 0 ; i < size ; ++i)
    {
        wordsOccurences = wordsPair[i].getSecond();
        while(wordsOccurences)
        {
          addedWords.insert(wordsPair[i].getFirst());  
          wordsOccurences--;
        }
        
    }
    return addedWords;
}

std::vector<pair<std::string , int>> WordsMultiset::inorderWords()const
{
    return pairs.inorder();
}

pair<std::string , int>& WordsMultiset::find(pair<std::string , int>& searched)const
{
    return pairs.find(searched)->value;
}

void WordsMultiset::remove(pair<std::string , int> value)
{
    if(!pairs.empty())
    {   
        node<pair<std::string , int>>* current = pairs.find(value);
        if(current->value.getSecond() > value.getSecond())
        {
            current->value.getSecond() -= value.getSecond();
        }
        else
        {
            pairs.remove(value);
        }
        
    }
}

size_t WordsMultiset::numberOfAllWords()
{
    int count = 0;
    std::vector<pair<std::string , int>> wordsPairs = pairs.inorder();
    for(auto it = wordsPairs.begin() ; it != wordsPairs.end() ; ++it)
    {
        count += (*it).getSecond();
    }
    return count;
}

WordsMultiset Comparator::create(std::istream& data)
{
    std::string word;
    if(data.good())
    {
        WordsMultiset newWordMultiset;
        while(!data.eof())
        {
            data>>word;
            if (word.empty())
            {
                return newWordMultiset;
            }
            newWordMultiset.add(word);
        }
        return newWordMultiset;
    }
    throw std::invalid_argument("cant open the stream");
}


ComparisonReport Comparator::compare(std::istream& a, std::istream& b)
{
    WordsMultiset firstMultiset = create(a);
    WordsMultiset secondMultiset = create(b);
    ComparisonReport newCmpReport;
    pair<std::string , int>* currentWord = nullptr;
    std::vector<pair<std::string , int>> firstMultisetPairs = firstMultiset.inorderWords();
    for(auto it = firstMultisetPairs.begin() ; it != firstMultisetPairs.end() ; ++it)
    {
        currentWord = &(secondMultiset.find((*it)));
        if(currentWord != nullptr)
        {
            newCmpReport.commonWords.add((*it).getFirst() , std::min(currentWord->getSecond() , (*it).getSecond()));
            if((*it).getSecond() > currentWord->getSecond())
            {
                newCmpReport.uniqueWords[0].add((*it).getFirst() , (*it).getSecond() - currentWord->getSecond());
            }
            secondMultiset.remove((*it)); // erase common words in secondMultiset
        }
        else
        {
            newCmpReport.uniqueWords[0].add((*it).getFirst() ,  (*it).getSecond());
        }
    }
    // add everything that had left in secondMultiset after erasing common words
    std::vector<pair<std::string , int>> secondMultisetPairs = secondMultiset.inorderWords();
    for(auto it = secondMultisetPairs.begin() ; it != secondMultisetPairs.end() ; ++it)
    {
        newCmpReport.uniqueWords[1].add((*it).getFirst() ,  (*it).getSecond() );
    }

    return newCmpReport;
}



#endif