#include "buildTree.h"

store scores[11268];

int main()
{
	Trienode* searchRoot = nullptr, * stopword = nullptr;
	
	
	cout << "Building...";
	//start build tree
	for (long i = 0; i < 11268; i++)
	{
		scores[i].fileIndex = i;
	}

	searchRoot = new Trienode;
	for (long i = 0; i < 42; i++)
		searchRoot->character[i] = nullptr;
	searchRoot->isEnd = false;

	stopword = new Trienode;
	for (long i = 0; i < 42; i++)
		stopword->character[i] = nullptr;
	stopword->isEnd = false;

	updateFileData();

	build2Tree(searchRoot, stopword);
	
	//end build tree

	cout << "OK!" << endl;

	while (1)
	{
		for (long i = 0; i < 11268; i++)
		{
			scores[i].score = -1;
		}
		string query;
		getline(cin, query);
		query = '"' + query + '"';
		cout << query << endl;
		if (!searchAll(searchRoot, query, stopword, scores)) {
			cout << "have nothing";
			break;
		}
		sort(scores, scores + 11268, cmp);
		long ir = 0;
		while (scores[ir].score > 0)
		{
			cout << fileData[scores[ir].fileIndex] << endl;
			ir++;
		}
		long cont;
		cout << "CONTINUE: "; cin >> cont;
		if (!cont) break;
		cin.ignore();
	}
	
	//deallocated
	deleteTree(searchRoot);
	deleteTree(stopword);
	return 0;
}