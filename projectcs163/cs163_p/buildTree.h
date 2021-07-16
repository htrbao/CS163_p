#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <vector>
#include <ctime>
#include <fstream>

using namespace std;

struct Trienode {
	Trienode* character[42];
	bool isEnd;

	vector <pair<long, long> > dataIndex;
};

extern string fileData[12000];

void updateFileData();

void buildTree(Trienode*& searchTree, Trienode*& stopword);

void handlingFile(Trienode*& root, long fileIndex);

void handlingSentence(Trienode*& root, string sen, long& start, long fileIndex);

long convertIndex(char c);

void insert(Trienode*& root, string str, long start, long fileIndex);

void insertStopword(Trienode*& stopword, string w);

Trienode* searchWord(Trienode* root, string word);

bool searchAll(Trienode* root, string query, Trienode* stopword, vector<long>& pos, long& searchScore);

void deleteTree(Trienode*& root);

//

bool isNumber(char c);

bool accept(char& c);

string senFilter(string sen);