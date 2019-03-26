// WordChecker.cpp
//
// ICS 46 Winter 2019
// Project #3: Set the Controls for the Heart of the Sun
//
// Replace and/or augment the implementations below as needed to meet
// the requirements.

#include "WordChecker.hpp"
#include <utility>
#include <string>


WordChecker::WordChecker(const Set<std::string>& words)
    : words{words}
{
}


bool WordChecker::wordExists(const std::string& word) const
{
    return words.contains(word);
}


std::vector<std::string> WordChecker::findSuggestions(const std::string& word) const
{
	std::vector<std::string> suggestions;
	std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	swapAdjacent(word, suggestions);
	insertChar(word, suggestions, alphabet);
	deleteChar(word, suggestions);
	replaceChar(word, suggestions, alphabet);
	splitWord(word, suggestions);

    return suggestions;
}


void WordChecker::swapAdjacent(const std::string& word, std::vector<std::string>& suggestions) const
{
	for (int i = 0; i < word.size() - 1; ++i)
	{
		std::string temp = word;
		std::swap(temp[i], temp[i + 1]);

		if (wordExists(temp) && notContains(temp, suggestions))
		{
			suggestions.push_back(temp);
		}
	}
}

void WordChecker::insertChar(const std::string& word, std::vector<std::string>& suggestions, const std::string& alphabet) const
{
	for (int i = 0; i < word.size() + 1; ++i)
	{
		for (int j = 0; j < alphabet.size(); ++j)
		{
			std::string temp = word;
			temp.insert(i, alphabet.substr(j, 1));

			if (wordExists(temp) && notContains(temp, suggestions))
			{
				suggestions.push_back(temp);
			}
		}
	}
}

void WordChecker::deleteChar(const std::string& word, std::vector<std::string>& suggestions) const 
{
	for (int i = 0; i < word.size(); ++i)
	{
		std::string temp = word;
		temp.erase(i, 1);
		if (wordExists(temp) && notContains(temp, suggestions))
		{
			suggestions.push_back(temp);
		}
	}
}

void WordChecker::replaceChar(const std::string& word, std::vector<std::string>& suggestions, const std::string& alphabet) const
{
	for (int i = 0; i < word.size(); ++i)
	{
		std::string temp = word;

		for (int j = 0; j < alphabet.size(); ++j)
		{
			temp.replace(i, 1, alphabet.substr(j, 1));
			if (wordExists(temp) && notContains(temp, suggestions))
			{
				suggestions.push_back(temp);
			}
		}
	}
}

void WordChecker::splitWord(const std::string& word, std::vector<std::string>& suggestions) const
{
	for (int i = 1; i < word.size(); ++i)
	{
		std::string left = word.substr(0, i);
		std::string right = word.substr(i);

		if (wordExists(left) && wordExists(right) && notContains(left + " " + right, suggestions))
		{
			suggestions.push_back(left + " " + right);
		}
	}
}

bool WordChecker::notContains(const std::string& word, std::vector<std::string>& suggestions) const
{
	return std::find(suggestions.begin(), suggestions.end(), word) == suggestions.end();
}