#include "buildTree.h"

string fileData[12000];

void updateFileData()
{
	ifstream ifs;
	ifs.open("Search-Engine-Data/___index.txt");
	long i = 0;
	while (!ifs.eof())
	{
		string s;

		getline(ifs, s, '\n');
		fileData[i++] = s;
	}
	ifs.close();
}

void buildTree(Trienode*& searchTree, Trienode*& stopword)
{
	//build SearchTree
	for (long i = 0; i < 11268; i++)
	{
		handlingFile(searchTree, i);
		cout << "build ok file " << i << endl;
	}
	//build stopwordTree
	ifstream ifs;
	ifs.open("Search-Engine-Data/___stopword.txt");
	if (!ifs.is_open())
	{
		cout << "Error at building stopword" << endl;
		return;
	}
	while (!ifs.eof())
	{
		string w;
		ifs >> w;
		insertStopword(stopword, w);
	}
}

void handlingFile(Trienode*& root, long fileIndex)
{
	ifstream ifs;
	string sen;
	ifs.open("Search-Engine-Data/" + fileData[fileIndex]);
	if (!ifs.is_open()) {
		cout << "Error at " << fileIndex << "th ! " << fileData[fileIndex];
		deleteTree(root);
		cout << "DeleteOK";
	}
	long start = 0;
	while (!ifs.eof())
	{
		getline(ifs, sen, '.'); //we get the sentence when meet .
		if (sen.length() && isNumber(sen.back())) {
			string next; 
			getline(ifs, next, '.');
			if (next.length() && isNumber(next[0])) {
				sen = sen + '.' + next;
				handlingSentence(root, sen, start, fileIndex);
				continue;
			}
			else {
				handlingSentence(root, sen, start, fileIndex);
				handlingSentence(root, next, start, fileIndex);
				continue;
			}
		}
		handlingSentence(root, sen, start, fileIndex);
	}
}

void handlingSentence(Trienode*& root, string sen, long& start, long fileIndex)
{
	sen = senFilter(sen);
	stringstream ss (sen);
	while (ss >> sen)
	{
		insert(root, sen, start, fileIndex);
		start++;
	}
}

long convertIndex(char c)
{
	if ('A' <= c && c <= 'Z') c += 32;
	if ('a' <= c && c <= 'z') return c - 'a';
	if ('0' <= c && c <= '9') return c - 22;
	if (c == ' ') return 36;
	if (c == '.') return 37;
	if (c == '$') return 38;
	if (c == '%') return 39;
	if (c == '#') return 40;
	if (c == '-') return 41;
	return -1;
}

void insert(Trienode*& root, string str, long start,long fileIndex)
{
	//cout << long(str[str.size()-1]);
	Trienode* cur = root;
	for (long i = 0; i < str.size(); i++)
	{
		long index = convertIndex(str[i]);
		if (index == -1) continue;
		if (cur->character[index]) cur = cur->character[index];
		else {
			cur->character[index] = new Trienode;
			cur = cur->character[index];
	
			for (long i = 0; i < 42; i++)
				cur->character[i] = nullptr;
			cur->isEnd = false;
		}
	}

	cur->dataIndex.push_back({ start, fileIndex });
	cur->isEnd = true;
}

void insertStopword (Trienode*& stopword, string w)
{
	Trienode* cur = stopword;
	for (long i = 0; i < w.size(); i++)
	{
		long index = convertIndex(w[i]);
		if (index == -1) continue;
		if (cur->character[index]) cur = cur->character[index];
		else {
			cur->character[index] = new Trienode;
			cur = cur->character[index];

			for (long i = 0; i < 42; i++)
				cur->character[i] = nullptr;
		}
	}
	cur->isEnd = true;
}

Trienode* searchWord(Trienode* root, string word) {
	Trienode* cur = root;
	for (long i = 0; i < word.size(); i++)
	{
		long index = convertIndex(word[i]);
		if (index == -1) continue;
		cur = cur->character[index];
	}

	if (!cur || !(cur->isEnd)) return nullptr;
	return cur;
}

/*bool searchAll(Trienode* root, string query, Trienode* stopword, vector<long>& pos, long& searchScore)
{
	stringstream ss (query);
}*/

void deleteTree(Trienode*& root)
{
	if (!root) return;

	for (long i = 0; i < 42; i++)
	{
		deleteTree(root->character[i]);
	}

	delete root;
	root = nullptr;
}

//

bool isNumber(char c)
{
	return '0' <= c && c <= '9';
}

bool accept(char& c)
{
	if ('A' <= c && c <= 'Z') c += 32;
	if (c == '\n') c = ' ';
	if (c == '?') c = '-';
	if (('a' <= c && c <= 'z') || ('0' <= c && c <= '9')) return true;
	if (c == ' ' || c == '.' || c == '$' || c == '%' || c == '#' || c == '-') return true;
	return false;
}

string senFilter(string sen)
{
	string res = "";
	for (long i = 0; i <= sen.size(); i++)
	{
		if (!accept(sen[i]))
		{
			if (sen[i] == 39 && sen[i + 1] == 's') i++;
			else if(sen[i] == '.' && isNumber(sen[i - 1]) && isNumber(sen[i + 1])) res.append(sen, i, 1);
		}
		else res.append(sen, i, 1);
	}
	return res;
}