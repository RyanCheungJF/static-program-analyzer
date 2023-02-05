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

vector<tuple<string, string, string>> extractParameters(string s)
{
	vector<tuple<string, string, string>> res;
	int endOfString = s.size();
	int curIndex = 0;
	string param1;
	string param2;
	string param3;
	int bracCount;
	char curChar;
	while (curIndex < endOfString) {
		param1 = "";
		param2 = "";
		param3 = "";
		bracCount = 0;
		curChar = '\0';
		while (curIndex < endOfString) {
			curChar = s[curIndex];
			if (curChar == '(') {
				bracCount++;
				break;
			}
			param1 += curChar;
			curIndex++;
		}
		curIndex++;
		while (curIndex < endOfString) {
			curChar = s[curIndex];
			if (curChar == ',') {
				break;
			}
			param2 += curChar;
			curIndex++;
		}
		curIndex++;
		while (curIndex < endOfString) {
			curChar = s[curIndex];
			if (curChar == '(') {
				bracCount++;
			}
			if (curChar == ')') {
				bracCount--;
				if (bracCount == 0) break;
			}
			param3 += curChar;
			curIndex++;
		}
		if (param1 == "" || param2 == "" || param3 == "" || curChar != ')') {
			throw - 1;
		}
		//validate param3 first
		param3 = removeCharFromString(param3, '\"');
		res.push_back(tuple<string, string, string>(param1, param2, param3));
		curIndex++;
	}
	return res;
}

string removeCharFromString(string s, char c) {
	s.erase(remove(s.begin(), s.end(), c), s.end());
	return s;
}
tuple<string, size_t> extractSubStringUntilDelimiter(const string& original, int start, string delimiter)
{
	size_t end = original.find(delimiter, start);
	if (end == string::npos) {
		return tuple<string, int>(original.substr(start, original.size()), original.size());
	}
	size_t length = end - start;
	string substr = original.substr(start, length);
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


