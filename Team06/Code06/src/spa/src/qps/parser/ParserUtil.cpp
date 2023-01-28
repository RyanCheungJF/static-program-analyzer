#include "ParserUtil.h"

/*
returns index of "such" which is followed immediately by a "that" in the word list

*/
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

vector<string> stringToWordList(string s) {
	vector<string> wordList;
	istringstream ss(s);
	string word;
	while (ss >> word) {
		wordList.push_back(word);
	}
	return wordList;
}
