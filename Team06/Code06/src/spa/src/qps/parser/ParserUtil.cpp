#include "ParserUtil.h"
#include <iostream>

/*
returns index of "such" which is followed immediately by a "that" in the word list
 */

const string WHITESPACE = " \n\r\t\f\v";

long findSuchThat(const vector<string> &wordList)
{
	string such = "such";
	string that = "that";
	for (int i = 0; i < wordList.size(); i++) {
		if (such != wordList[i]) {
			continue;
		}
		if (i == wordList.size() - 1) {
			continue;
		}
		if (that != wordList[i + 1]) {
			continue;
		}
		return i;
	}

	return -1;
}

long findPattern(const vector<string> &wordList)
{
	for (int i = 0; i < wordList.size(); i++) {
		if (!isPattern(wordList[i])) {
			continue;
		}
		if (i == wordList.size() - 1) {
			continue;
		}
		if (!startsWithLetter(wordList[i + 1])) {
			continue;
		}
		return i;
	}
	return -1;
}

tuple<string, size_t> extractSubStringUntilDelimiter(const string& original, int start, string delimiter)
{
	size_t end = original.find(delimiter, start);
	if (end == string::npos) {
		throw Exception();
	}
	string substr = original.substr(start, end);
	return tuple<string, int>(substr, end + 1);
}

vector<string> stringToWordList(string s) {
	vector<string> wordList;
	stringstream ss(s);
	string word;
	while (ss >> word) {
		wordList.push_back(word);
	}
	return wordList;
}

string ltrim(const std::string &s) {
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}

string rtrim(const std::string &s) {
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

string trim(const std::string &s) {
    return rtrim(ltrim(s));
}

vector<string> stringToWordListByDelimiter(string original, string delimiter) {
    vector<string> wordList;
    size_t length = original.length();
    while (length != 0) {
        size_t index = original.find(delimiter);
        if (index == string::npos) {
            wordList.push_back(trim(original));
            break;
        }
        string word = original.substr(0, index);
        wordList.push_back(trim(word));
        if(index + 1 < length) {
            original = original.substr(index + 1, length);
            length = original.length();
        } else {
            break;
        }
    }
    return wordList;
}
