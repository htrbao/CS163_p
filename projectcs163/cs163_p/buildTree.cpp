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

void build2Tree(Trienode*& searchTree, Trienode*& stopword)
{
	//build SearchTree
	//for (long i = 0; i < 11268; i++)
	for (long i = 0; i < 10; i++)
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
		if (!cur->character[index]) return nullptr;
		cur = cur->character[index];
	}

	if (!cur || !(cur->isEnd)) return nullptr;
	return cur;
}

void exact(vector <long>& a1, vector<long>& a2, long cnt, vector<long>& out1, vector<long>& out2)
{
	long i = 0, j = 0;
	while (i < a1.size() && j < a2.size())
	{
		if (a1[i] + cnt < a2[j]) i++;
		else if (a2[j] + cnt < a1[i]) j++;
		else
		{
			out1.push_back(a1[i++]);
			out2.push_back(a2[j++]);
		}
	}
}
//for exact
void handleWord(vector<pair<long, long> > res1, vector<pair<long, long> > res2, long cnt, store score[])
{
	long i = 0, j = 0;
	while (i < res1.size() && j < res2.size())
	{
		if (score[res1[i].second].score == -1) {
			i++;
			continue;
		}
		if (score[res2[j].second].score == -1) {
			j++;
			continue;
		}
		if (res1[i].second != res2[j].second) {
			while (res1[i].second < res2[j].second) {
				score[res1[i].second].score = -1;
				i++;
			}
			while (res2[j].second < res1[i].second) {
				score[res2[j].second].score = -1;
				j++;
			}
		}
		if (res1[i].second == res2[j].second)
		{
			long hld = res1[i].second;
			vector <long> a1, a2;
			while (i < res1.size() && res1[i].second == hld)
			{
				a1.push_back(res1[i++].first); 
			}
			while (j < res2.size() && res2[j].second == hld)
			{
				a2.push_back(res2[j++].first);
			}
			vector<long> out1, out2;

			exact(a1, a2, cnt, out1, out2);

			if (out1.size() == 0) {
				score[hld].score = -1;
				continue;
			}
			else {
				for (long kk = 0; kk < out1.size(); kk++) {
					score[hld].pos.insert(out1[kk]);
					score[hld].pos.insert(out2[kk]);
					score[hld].score += 2;
				}
			}
		}
	}
}

bool searchAll(Trienode* root, string query, Trienode* stopword, store score[])
{
	stringstream ss(query);
	string tmp;
	while (ss >> tmp)
	{
		if (tmp[0] == '"')
		{
			Trienode* res1 = searchWord(root, tmp);
			if (!res1) return false;

			for (long i = 0; i < res1->dataIndex.size(); i++)
				score[res1->dataIndex[i].second].score = 0;

			if (tmp.back() == '"') {
				for (long i = 0; i < res1->dataIndex.size(); i++)
				{
					score[res1->dataIndex[i].second].fileIndex = res1->dataIndex[i].second;
					score[res1->dataIndex[i].second].score++;
					score[res1->dataIndex[i].second].pos.insert(res1->dataIndex[i].first);
				}
				continue;
			}
			string tmp2;
			while (ss >> tmp2)
			{
				long cnt = 1;
				if (tmp2 == "") {
					return true;
				}
				if (tmp2 == "*") {
					cnt++;
					while (ss >> tmp2) {
						if (tmp2 == "*") cnt++;
						else break;
					}
				}
				Trienode* res2 = searchWord(root, tmp2);
				if (!res2) return false;
				handleWord(res1->dataIndex, res2->dataIndex, cnt, score);
				bool flag = true;
				for (long i = 0; i <= 11268; i++)
					if (score[i].score != -1) flag = false;
				if (flag) return false;
				if (tmp2.back() == '"') {
					break;
				}
				res1 = res2;
			}
		}
	}
	return true;
}

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

bool cmp(store& a, store& b)
{
	return a.score > b.score;
}