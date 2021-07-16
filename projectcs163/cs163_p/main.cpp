#include "buildTree.h"



int main()
{
	Trienode* searchRoot = nullptr, * stopword = nullptr;
	
	cout << "Building...";
	//start build tree
	searchRoot = new Trienode;
	for (long i = 0; i < 42; i++)
		searchRoot->character[i] = nullptr;
	searchRoot->isEnd = false;

	stopword = new Trienode;
	for (long i = 0; i < 42; i++)
		stopword->character[i] = nullptr;
	stopword->isEnd = false;

	updateFileData();

	buildTree(searchRoot, stopword);
	
	//end build tree
	cout << "OK!";
	deleteTree(searchRoot);
	deleteTree(stopword);
	return 0;
}