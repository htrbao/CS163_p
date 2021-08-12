#include "buildTree.h"
#include "windowConsole.h"

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

	system("cls");
	
	//end build tree

	drawLogo();
	drawRec(1, 24, 50, 7);

	while (1)
	{
		clearSearch();
		for (long i = 0; i < 11268; i++)
		{
			scores[i].score = -1;
		}
		string query;
		gotoxy(52, 8);
		getline(cin, query);

		if (query == "history Of Search") {
			ifstream ifs;
			ifs.open("Search-Engine-Data/___history.txt");
			long i = 0;
			while (!ifs.eof()) {
				string outp;
				getline(ifs, outp);
				if (outp == "") continue;
				gotoxy(55, 11 + i); cout << ++i << ". " << outp << endl;
			}
			long cont;
			ifs.close();
			cout << "CONTINUE: "; cin >> cont;
			if (!cont) break;
			cin.ignore();
			continue;
		}
		/*if(checkOperator(query)) 
			query = '"' + query + '"';*/
		else {
			ofstream ofs;
			ofs.open("Search-Engine-Data/___history.txt", ios::app);
			ofs << query << endl;
		}
		clock_t start = clock();
		if (!searchAll(searchRoot, query, stopword, scores)) {
			gotoxy(10, 11); cout << "have nothing";
			long cont;
			cout << "CONTINUE: "; cin >> cont;
			if (!cont) break;
			cin.ignore();
			continue;
		}
		sort(scores, scores + 11268, cmp);
		clock_t end = clock();
		long ir = 0;
		gotoxy(51, 10);
		cout << "(" << double(end - start) / CLOCKS_PER_SEC << " seconds)" << endl;
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