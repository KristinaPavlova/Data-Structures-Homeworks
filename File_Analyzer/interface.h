#ifndef INTERFACE_H
#define INTERFACE_H
#include <iostream>
#include <set>
#include <string>
#include"avlTree.h"
#include"pair.h"
#include<vector>

class WordsMultiset {
public:
	WordsMultiset() = default;
	WordsMultiset(const WordsMultiset&);
	WordsMultiset& operator=(const WordsMultiset&);
	/// Adds times occurences of word to the container
	///
	/// For example, add("abc") adds the word "abc" once,
	/// while add("abc", 4) adds 4 occurrances.
	void add(const std::string& word, size_t times = 1); //ok

	/// Checks whether word is contained in the container
	bool contains(const std::string& word) const;

	/// Number of occurrances of word 
	size_t countOf(const std::string& word) const;

	/// Number of unique words in the container
	size_t countOfUniqueWords() const;

	/// Returns a multiset of all words in the container
	std::multiset<std::string> words() const;
	
	// You can add additional members if you need to

	std::vector<pair<std::string , int>> inorderWords()const;

	// returns pointer to the element if found else return nullptr
	pair<std::string , int>& find(pair<std::string , int>&)const; 

	void remove(pair<std::string , int>);

	size_t numberOfAllWords();

    private:

    avlTree<pair<std::string , int>> pairs;
};

///
/// Results of the comparison of two streams of words
/// DO NOT modify this class
/// If you need to see how it is intended to be used,
/// check out the corresponding unit tests
///
class ComparisonReport {
public:
	/// A multiset of all words that exist in both streams
	WordsMultiset commonWords;

	/// Multisets of words unique to the two streams
	/// The first element of the array contains the words that are unique
	/// to the first stream (a) and the second one -- to the second stream (b)
	WordsMultiset uniqueWords[2];
};

/// 
/// Can be used to compare two streams of words
///
class Comparator {
public:
	WordsMultiset create(std::istream& a);

	ComparisonReport compare(std::istream& a, std::istream& b);

};

#endif