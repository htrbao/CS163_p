#include "buildTree.h"
#include "windowConsole.h"

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

Trienode* searchWordForIncomplete(Trienode* root, string word) {
	Trienode* cur = root;
	for (long i = 0; i < word.size(); i++)
	{
		long index = convertIndex(word[i]);
		if (index == -1) continue;
		if (!cur->character[index]) return nullptr;
		cur = cur->character[index];
	}

	return cur;
}

void searchIncomplete(Trienode* root, store score[])
{
	if (!root) return;
	if (root->isEnd) {
		for (long kk = 0; kk < root->dataIndex.size(); kk++) {
			score[root->dataIndex[kk].second].fileIndex = root->dataIndex[kk].second;
			score[root->dataIndex[kk].second].score += (score[root->dataIndex[kk].second].score == -1 ? 2 : 1);
			score[root->dataIndex[kk].second].pos.insert(root->dataIndex[kk].first);
		}
	}
	for (long i = 0; i < 42; i++) {
		if (root->character[i] != nullptr) {
			searchIncomplete(root->character[i], score);
		}
	}
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

vector <string> getSyn(string tmp) {
	vector <string> res;
	if ('a' <= tmp[0] && tmp[0] <= 'z') tmp[0] -= 32;
	for (long i = 1; i < tmp.size(); i++)
	{
		if ('A' <= tmp[i] && tmp[i] <= 'Z') tmp[i] += 32;
	}
	ifstream ifs;
	ifs.open("Search-Engine-Data/___synant.txt");
	string cur;
	while (!ifs.eof()) {
		getline(ifs, cur);
		stringstream ss(cur);
		ss >> cur;
		if (cur != "KEY:") continue;
		ss >> cur;
		if (cur != tmp) continue;
		if (cur > tmp) return res;
		getline(ifs, cur);
		if (cur == "") getline(ifs, cur);;
		stringstream st(cur);
		st >> cur;
		if (cur != "SYN:") break;
		while (st >> cur)
		{
			res.push_back(cur);
		}
		ifs.close();
		return res;
	}
	ifs.close();
	return res;
}

vector <string> getAnt(string tmp) {
	vector <string> res;
	if ('a' <= tmp[0] && tmp[0] <= 'z') tmp[0] -= 32;
	for (long i = 1; i < tmp.size(); i++)
	{
		if ('A' <= tmp[i] && tmp[i] <= 'Z') tmp[i] += 32;
	}
	ifstream ifs;
	ifs.open("Search-Engine-Data/___synant.txt");
	string cur;
	while (!ifs.eof()) {
		getline(ifs, cur);
		stringstream ss(cur);
		ss >> cur;
		if (cur != "KEY:") continue;
		ss >> cur;
		if (cur != tmp) continue;
		if (cur > tmp) return res;
		getline(ifs, cur);
		getline(ifs, cur);
		if (cur == "") getline(ifs, cur);
		stringstream st(cur);
		st >> cur;
		if (cur != "ANT:") break;
		while (st >> cur)
		{
			res.push_back(cur);
		}
		ifs.close();
		return res;
	}
	ifs.close();
	return res;
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
				if(score[res1->dataIndex[i].second].score == -1)
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
		if (tmp[0] == '-') {//minus case
			Trienode* cur = searchWord(root, tmp);
			if (cur) {
				for (long kk = 0; kk < cur->dataIndex.size(); kk++)
				{
					score[cur->dataIndex[kk].second].score = -1000;	
				}
			}
			continue;
		}
		Trienode* searchRes;
		if (tmp == "OR") {
			ss >> tmp;
			searchRes = searchWord(root, tmp);
			if (searchRes) {
				for (long kk = 0; kk < searchRes->dataIndex.size(); kk++)
				{
					score[searchRes->dataIndex[kk].second].fileIndex = searchRes->dataIndex[kk].second;
					score[searchRes->dataIndex[kk].second].score += (score[searchRes->dataIndex[kk].second].score == -1 ? 2 : 1);
					score[searchRes->dataIndex[kk].second].pos.insert(searchRes->dataIndex[kk].first);
				}
			}
			continue;
		}
		searchRes = searchWord(root, tmp);
		if (tmp[0] == '+') {
			if (!searchRes) return false;
			for (long kk = 0; kk < searchRes->dataIndex.size(); kk++)
			{
				score[searchRes->dataIndex[kk].second].fileIndex = searchRes->dataIndex[kk].second;
				score[searchRes->dataIndex[kk].second].score += (score[searchRes->dataIndex[kk].second].score == -1 ? 2 : 1);
				score[searchRes->dataIndex[kk].second].pos.insert(searchRes->dataIndex[kk].first);
			}
		}
		if (tmp[tmp.size() - 1] == '*') {
			searchRes = searchWordForIncomplete(root, tmp);
			if (searchRes) searchIncomplete(searchRes, score);
		}
		if (tmp[0] == '~') {
			vector <string> s = getSyn(tmp.substr(1));
			for (long hh = 0; hh < s.size(); hh++)
			{
				searchRes = searchWord(root, s[hh]);
				if (searchRes) {
					for (long kk = 0; kk < searchRes->dataIndex.size(); kk++)
					{
						score[searchRes->dataIndex[kk].second].fileIndex = searchRes->dataIndex[kk].second;
						score[searchRes->dataIndex[kk].second].score += (score[searchRes->dataIndex[kk].second].score == -1 ? 2 : 1);
						score[searchRes->dataIndex[kk].second].pos.insert(searchRes->dataIndex[kk].first);
					}
				}
			}
		}
		if (tmp[0] == '!') {
			vector <string> s = getAnt(tmp.substr(1));
			for (long hh = 0; hh < s.size(); hh++)
			{
				searchRes = searchWord(root, s[hh]);
				if (searchRes) {
					for (long kk = 0; kk < searchRes->dataIndex.size(); kk++)
					{
						score[searchRes->dataIndex[kk].second].fileIndex = searchRes->dataIndex[kk].second;
						score[searchRes->dataIndex[kk].second].score += (score[searchRes->dataIndex[kk].second].score == -1 ? 2 : 1);
						score[searchRes->dataIndex[kk].second].pos.insert(searchRes->dataIndex[kk].first);
					}
				}
			}
		}
		if (searchWord(stopword, tmp)) continue;
		if (tmp == "AND" || tmp == "filetype:txt") continue;
		
		if (!searchRes) {
			bool isOr = false;
			while (ss >> tmp) {
				if (tmp == "OR") {
					ss >> tmp;
					searchRes = searchWord(root, tmp);
					if (searchRes) {
						isOr = true;
						for (long kk = 0; kk < searchRes->dataIndex.size(); kk++) {
							score[searchRes->dataIndex[kk].second].fileIndex = searchRes->dataIndex[kk].second;
							score[searchRes->dataIndex[kk].second].score += (score[searchRes->dataIndex[kk].second].score == -1 ? 2 : 1);
							score[searchRes->dataIndex[kk].second].pos.insert(searchRes->dataIndex[kk].first);
						}
						break;
					}
				}
				else return false;
			}
			if (!isOr) return false;
		}
		else {
			for (long kk = 0; kk < searchRes->dataIndex.size(); kk++)
			{
				score[searchRes->dataIndex[kk].second].fileIndex = searchRes->dataIndex[kk].second;
				score[searchRes->dataIndex[kk].second].score += (score[searchRes->dataIndex[kk].second].score == -1 ? 2 : 1);
				score[searchRes->dataIndex[kk].second].pos.insert(searchRes->dataIndex[kk].first);
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

bool checkOperator(string query)
{
	stringstream ss(query);
	string tmp;
	while (ss >> tmp) {
		if (tmp.substr(0, 8) == "intitle:") return false;
		if (tmp == "AND" || tmp == "OR" || tmp[0] == '-' || tmp == "*" || tmp[0] == '"' || tmp == "filetext:txt" || tmp.back() == '*') return false;
		for (long kk = 0; kk < tmp.size() - 2; kk++) {
			if (tmp[kk] == '.' && tmp[kk + 1] == '.') return false;
		}
	}
	return true;
}

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

void drawLogo()
{
	long a = 40;
	gotoxy(25 + a, 0); setTextColor(9); cout << "  _________";
	gotoxy(25 + a, 1); setTextColor(9); cout << " /   _____/";
	gotoxy(25 + a, 2); setTextColor(9); cout << " \\_____  \\";
	gotoxy(25 + a, 3); setTextColor(9); cout << " /        \\";
	gotoxy(25 + a, 4); setTextColor(9); cout << "/_______  /";
	gotoxy(25 + a, 5); setTextColor(9); cout << "        \\/";

	gotoxy(36 + a, 0); setTextColor(12); cout << "___________";
	gotoxy(36 + a, 1); setTextColor(12); cout << "\\_   _____/";
	gotoxy(36 + a, 2); setTextColor(12); cout << " |    __)_";
	gotoxy(36 + a, 3); setTextColor(12); cout << " |        \\";
	gotoxy(36 + a, 4); setTextColor(12); cout << "/_______  /";
	gotoxy(36 + a, 5); setTextColor(12); cout << "        \\/";

	gotoxy(47 + a, 0); setTextColor(14); cout << "___________";
	gotoxy(47 + a, 1); setTextColor(14); cout << "\\_   _____/";
	gotoxy(47 + a, 2); setTextColor(14); cout << " |    __)_";
	gotoxy(47 + a, 3); setTextColor(14); cout << " |        \\";
	gotoxy(47 + a, 4); setTextColor(14); cout << "/_______  /";
	gotoxy(47 + a, 5); setTextColor(14); cout << "        \\/";

	gotoxy(58 + a, 0); setTextColor(9); cout << "____   ____";
	gotoxy(58 + a, 1); setTextColor(9); cout << "\\   \\ /   /";
	gotoxy(58 + a, 2); setTextColor(9); cout << " \\   Y   / ";
	gotoxy(58 + a, 3); setTextColor(9); cout << "  \\     /  ";
	gotoxy(58 + a, 4); setTextColor(9); cout << "   \\___/   ";

	gotoxy(69 + a, 0); setTextColor(10); cout << ".____     ";
	gotoxy(69 + a, 1); setTextColor(10); cout << "|    |    ";
	gotoxy(69 + a, 2); setTextColor(10); cout << "|    |    ";
	gotoxy(69 + a, 3); setTextColor(10); cout << "|    |___ ";
	gotoxy(69 + a, 4); setTextColor(10); cout << "|_______ \\";
	gotoxy(69 + a, 5); setTextColor(10); cout << "        \\/";

	gotoxy(79 + a, 0); setTextColor(12); cout << "___________";
	gotoxy(79 + a, 1); setTextColor(12); cout << "\\_   _____/";
	gotoxy(79 + a, 2); setTextColor(12); cout << " |    __)_";
	gotoxy(79 + a, 3); setTextColor(12); cout << " |        \\";
	gotoxy(79 + a, 4); setTextColor(12); cout << "/_______  /";
	gotoxy(79 + a, 5); setTextColor(12); cout << "        \\/";

	cout << endl;

	setTextColor(7);
}