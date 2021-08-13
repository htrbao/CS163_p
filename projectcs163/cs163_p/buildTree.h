#ifndef _BUILDTREE_H_

#define _BUILDTREE_H_

#include <iostream>
#include <time.h>
#include <string>
#include <set>
#include <algorithm>
#include <sstream>
#include <vector>
#include <ctime>
#include <fstream>
#include <utility>

#define edgge pair<int,int>

using namespace std;

struct Trienode {
	Trienode* character[42];
	bool isEnd;

	vector <pair<long, long> > dataIndex;
};

struct store {
	long fileIndex, score = 0;
	set <long> pos;
};

extern string fileData[12000];

void updateFileData();

void build2Tree(Trienode*& searchTree, Trienode*& stopword);

void handlingFile(Trienode*& root, long fileIndex);

void handlingSentence(Trienode*& root, string sen, long& start, long fileIndex);

long convertIndex(char c);

void insert(Trienode*& root, string str, long start, long fileIndex);

void insertStopword(Trienode*& stopword, string w);

Trienode* searchWord(Trienode* root, string word);

void searchIncomplete(Trienode* root, store score[]);

void exact(vector <long>& a1, vector<long>& a2, long cnt, vector<long>& out1, vector<long>& out2);

void handleWord(vector<pair<long, long> > res1, vector<pair<long, long> > res2, long cnt, store score[]);

vector <string> getSyn(string tmp);

vector <string> getAnt(string tmp);

bool searchAll(Trienode* root, string query, Trienode* stopword, store score[]);

void deleteTree(Trienode*& root);

//

bool checkOperator(string query);

bool isNumber(char c);

bool accept(char& c);

string senFilter(string sen);

bool cmp(store& a, store& b);

void drawLogo();

bool isSearchWithinARange(string s);

pair<string, edgge> rangeBetweenTwoStr_(string s);

#endif // !_BUILDTREE_H_
