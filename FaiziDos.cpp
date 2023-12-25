#include<iostream>
#include<ctime>
#include<vector>
#include<queue>
#include<string>
#include<windows.h>
#include<string>
#include<sstream>
#include<conio.h>
#include<list>

using namespace std;
// NOTEPAD
class Notepad {
public:
	list<char> temp;
	list<list<char>>* sheet;
	string tempString = "";
	Notepad(string data)
	{
		sheet = new list<list<char>>;
		load(data);
	}
	void load(string data)
	{
		int size = data.length();
		for (int i = 0; i < size; i++)
		{
			if (data[i] == '\n' || i == size - 1)
			{
				sheet->push_back(temp);
				temp.clear();
			}
			else
			{
				temp.push_back(data[i]);
			}
		}
	}
	void insertAtIndex(int index, string data) {
		list<list<char>>* newSheet = new list<list<char>>();
		int x = 0;
		for (auto i : *sheet)
		{
			if (x == index)
			{
				stringToList(data);
				newSheet->push_back(temp);
			}
			else
			{
				newSheet->push_back(i);
			}
			x++;
		}
		sheet = newSheet;
	}
	void insert(string data) {
		stringToList(data);
		sheet->push_back(temp);
	}
	void edit(int index)
	{
		int x = 0;
		for (auto i : *sheet)
		{
			if (x == index)
			{
				temp = i;
				break;
			}
			x++;
		}
	}
	void stringToList(string data)
	{
		temp.clear();
		for (int i = 0; i < data.length(); i++)
		{
			temp.push_back(data[i]);
		}
	}
	string listToString()
	{
		string data = "";
		for (auto i : temp)
		{
			data = data + i;
		}
		tempString = data;
		return data;
	}
	string listToString(list<char> temp)
	{
		string data = "";
		for (auto i : temp)
		{
			data = data + i;
		}
		return data;
	}
	string whole()
	{
		string data = "";
		for (auto i : *sheet)
		{
			data = data + listToString(i) + '\n';
		}
		return data;
	}
};

class FrontEnd {
public:
	Notepad* note;
	string select = "- 1";
	int EditedIndex = -1;
	list<char> work;
	vector<string> stateOfTime;
	vector<string> menu = { "Insert a Line","Edit a Line","Delete a Line","Replace a Line","Exit" };
	int state = 0;
	FrontEnd(string data)
	{
		note = new Notepad(data);
	}
	string Driver()
	{
		LinesPrint();
		while (true)
		{
			menuPrint(menu, 5, 5);
			if (select == "1")
			{

				note->insert(Lineget());
				LinesPrint();
			}
			if (select == "2")
			{
				edit();
				LinesPrint();
			}
			if (select == "3")
			{
				Delete();
				LinesPrint();
			}
			if (select == "4")
			{
				replace();
				LinesPrint();
			}
			if (select == "5")
			{
				cout << note->whole();
				return note->whole();
			}
			select = "- 1";
		}
		return "";
	}
	// Edit 1st part 
	void edit()
	{

		cin.ignore();
		gotoxy(5, 25);
		cout << "SelectLine: ";
		gotoxy(5 + 13, 25);
		cin >> EditedIndex;
		EditedIndex = EditedIndex - 1;
		note->edit(EditedIndex);
		EditingMode();
	}
	// Edit second part 
	void EditingMode()
	{

		string tempData = note->listToString() + ' ';
		int position = 0;
		string dm = "";
		int post = 0;
		int counter = 0;
		int lastCount = -1;
		bool copierState = false;
		PrintList(tempData, position);
		while (true)
		{
			// undo
			if (GetAsyncKeyState(VK_F1))
			{
				if (counter > 0)
					counter--;
				tempData = stateOfTime[counter];
				gotoxy(18, 25);
				position = 0;
				PrintList(tempData, position);
			}
			// REDO
			if (GetAsyncKeyState(VK_F2))
			{
				if (counter < stateOfTime.size())
					counter++;
				tempData = stateOfTime[counter];
				gotoxy(18, 25);
				position = 0;
				PrintList(tempData, position);
			}
			if (GetAsyncKeyState(VK_LEFT))
			{
				if (copierState)
				{
					gotoxy(1, 1);
					cout << "\033[35m" << "COPIED STATE ON";
					work.push_back(tempData[position]);
				}

				else
				{
					gotoxy(1, 1);
					cout << "                        ";
				}
				if (position > 0)
					position--;
				gotoxy(18, 25);
				cout << "                                                                                                               ";
				gotoxy(18, 25);
				PrintList(tempData, position);
			}
			else if (GetAsyncKeyState(VK_RIGHT))
			{

				if (copierState)
				{
					gotoxy(1, 1);
					cout << "\033[35m" << "COPIED STATE ON";
					work.push_back(tempData[position]);
				}
				else
				{
					gotoxy(1, 1);
					cout << "                        ";
				}
				if (position < tempData.length() - 1)
					position++;

				gotoxy(25, 25);
				cout << "                                                                                                               ";

				gotoxy(18, 25);
				PrintList(tempData, position);
			}
			else if (GetAsyncKeyState(VK_CONTROL))
			{
				stateOfTime.push_back(tempData);
				counter++;
				gotoxy(18, 24);
				getline(cin, dm);
				tempData.insert(position, dm);
				gotoxy(18, 25);
				PrintList(tempData, position);
				gotoxy(18, 24);
				cout << "                                                                                                                   ";
				stateOfTime.push_back(tempData);
				counter++;
			}
			else if (GetAsyncKeyState(0x41))
			{
				copierState = !copierState;
			}
			else if (GetAsyncKeyState(VK_BACK))
			{
				if (position > 0)
				{
					stateOfTime.push_back(tempData);
					counter++;
					tempData.erase(position - 1, 1);
					gotoxy(18, 25);
					PrintList(tempData, position);
					stateOfTime.push_back(tempData);
					counter++;
				}
			}
			else if (GetAsyncKeyState(VK_HOME))
			{
				tempData.insert(position, note->listToString(work));
				gotoxy(18, 25);
				PrintList(tempData, position);
				gotoxy(18, 24);
				work.clear();
			}
			else if (GetAsyncKeyState(VK_ESCAPE))
			{
				note->insertAtIndex(EditedIndex, tempData);
				EditedIndex = -1;
				return;
			}

			Sleep(200);
		}
	}
	// Replace
	void replace()
	{
		int x;
		gotoxy(25, 25);
		cout << "ENTER THE LINE NUMBER 1: ";
		gotoxy(53, 25);
		cin >> x;
		int y;
		gotoxy(25, 26);
		cout << "ENTER THE LINE NUMBER 2: ";
		gotoxy(53, 26);
		cin >> y;
		x--; y--;
		if (x >= 0 && x < note->sheet->size() && y >= 0 && y < note->sheet->size())
		{
			iter_swap(next(note->sheet->begin(), x), next(note->sheet->begin(), y));
		}

	}
	// delete 
	void Delete()
	{
		int x;
		gotoxy(25, 25);
		cout << "ENTER THE LINE NUMBER: ";
		gotoxy(50, 25);
		cin >> x;
		x = x - 1;
		if (x >= 0 && x < note->sheet->size())
		{
			auto it = next(note->sheet->begin(), x);
			note->sheet->erase(it);
		}
	}
	void setter()
	{

	}
	void PrintList(string data, int p)
	{
		int x = 0;
		for (char a : data)
		{
			if (p == x)
			{
				cout << "\033[32m" << a;
			}
			else
			{
				cout << "\033[31m" << a;
			}
			x++;
		}
	}
	string Lineget()
	{
		cin.ignore();
		string temp;
		gotoxy(5, 25);
		cout << "Enter: ";
		gotoxy(5 + 9, 25);
		getline(cin, temp);
		return temp;
	}
	string menuPrint(vector<string> arr, int x, int y)
	{
		for (int i = 0; i < arr.size(); i++)
		{
			gotoxy(x, y);
			cout << i + 1 << ". " << arr[i];
			y++;
		}
		int input;
		gotoxy(x, y++);
		cout << "Select: ";
		gotoxy(x, y++);
		cin >> select;
		return select;
	}
	void LinesPrint()
	{
		system("cls");
		int x = 1;
		for (const auto a : *(note->sheet))
		{
			gotoxy(120, x);
			cout << x << ": " << merger(a) << endl;
			x++;
		}

	}
	string merger(list<char> line)
	{
		string temp = "";
		for (auto a : line)
		{
			temp = temp + a;
		}
		return temp;
	}
	void gotoxy(int x, int y)
	{
		COORD coordinates;
		coordinates.X = x;
		coordinates.Y = y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinates);
	}

};



// DOS

class Folder;

enum position {
	lef, rig, up, down
};
enum What {
	file, folder
};

class BaseOfAll {
public:
	int size;
	string Name;
	string timet;
	string path;
	What Type;
	Folder* Father;
	BaseOfAll(int size, string Name, string path)
	{
		this->size = size;
		this->Name = Name;
		this->path = path;
		this->timet = getCurrentTime();
	}
	string getCurrentTime() {
		time_t rawtime;
		struct tm* timeinfo;
		char buffer[80];

		time(&rawtime);
		timeinfo = localtime(&rawtime);

		strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", timeinfo);
		std::string str(buffer);

		return str;
	}

	virtual ~BaseOfAll() = default; // Add a virtual destructor
	void SetFather(Folder* Father)
	{
		this->Father = Father;
	}
	bool ExtensionChecker(string name, int position)
	{
		if (position == -1)
		{
			return false;
		}
		if (name[position] == '.')
		{
			return true;
		}
		ExtensionChecker(name, position - 1);
	}
};

class File : public BaseOfAll {
public:
	string data;

	File(int size, string Name, string path, string data)
		: BaseOfAll(size, Name, path)
	{
		this->data = data;
		this->Type == file;
	}
	File(int size, string Name, string path)
		: BaseOfAll(size, Name, path)
	{

		this->data = "";
		this->Type == file;
	}
};

class Folder : public BaseOfAll {
public:
	vector<Folder*> FoldersInFolders;
	vector<File*> FilesInFolder;
	Folder(int size, string Name, string path)
		: BaseOfAll(size, Name, path)
	{
		this->Type = folder;
	}
};

class Dos {
public:
	vector<Folder*> Disk;
	vector<Folder*> Present;
	vector<File*> FilesPresentAll;
	Folder* PresentFolder;
	int level;
	Dos(int n)
	{
		level = 0;
		CreateDisk(n);
		Present = Disk;
	}
	void SetPresentFolder()
	{
		PresentFolder = Present[level];
	}
	// Inc & DEc
	bool IncDecDealer(position p)
	{
		if (p == lef)
		{
			if (level > 0)
			{
				level--;
				return true;
				// There is a bug which is knob as xoiw
			}
			else
			{
				return false;
			}
		}
		else if (p == rig)
		{
			if (level < Present.size() - 1)
			{
				level++;
				return true;
			}
			else
			{
				return false;
			}
		}

	}

	void InsertFile(File* F)
	{
		F->Father = PresentFolder;
		PresentFolder->FilesInFolder.push_back(F);
	}
	void InsertFolder(Folder* F)
	{
		F->Father = PresentFolder;
		PresentFolder->FoldersInFolders.push_back(F);

	}
	void CreateDisk(int size)
	{
		string name = "Local Disk ";
		char na = 'A';
		for (int i = 0; i < size; i++)
		{
			Folder* fl = new Folder(0, name + na, name + na + '/');
			na++;
			Disk.push_back(fl);
		}
	}
	void FolderSetter()
	{
		SetPresentFolder();
		Present = PresentFolder->FoldersInFolders;
		FilesPresentAll = PresentFolder->FilesInFolder;
		level = 0;
	}
	void moveTowardFather()
	{
		if (PresentFolder->Father != nullptr)
		{
			if (PresentFolder->Father->Father != nullptr)
			{
				Present = PresentFolder->Father->Father->FoldersInFolders;
				FilesPresentAll = PresentFolder->Father->Father->FilesInFolder;
			}
			else
			{
				Present = Disk;
				FilesPresentAll = vector<File*>{};
			}
			level = 0;
		}
	}
};

class DosFrontEnd {
	Dos* dos;
	int Line[2] = { 10,12 };
	vector<string> FileFolder = { "Folder" , "File" };
	vector<string> menuMain = { "ADD FILE","ADD FOLDER","DEL PRESENT","RENAME PRESENT","COPY","CUT","PAST","Tree Printer","Find","FINDSTR","PQUEUE","PWD","ATTRIB","CD SLASH","DIR","EXIT","FORMATE","HELP","Version","ADD IN QUEUE","QUEUE"};
	vector<string> CopyCutOpt = { "ADD FILE","ADD FOLDER","DEL PRESENT","RENAME PRESENT","COPY","CUT" };
	int menuSelector = -1;
	int FoldOrFilSelect = -1;
	vector<BaseOfAll*> CopiedOrCut;
	bool copyState = false;
	Folder* CopyFolder;
	bool terminater = false;
	queue<File*> fileToPrint;
	vector<BaseOfAll*> temp;
public:
	DosFrontEnd()
	{
		dos = new Dos(stoi(toGetAnyInput(10, 10, "Enter: ")));
	}
	// Dos Main Function Controller
	void DosWorkingAll()
	{
		print(10, 10);
		LinePrint(Line[0], Line[1]);
		while (true)
		{
			gotoxy(4, 4);
			cout << dos->level;
			if (GetAsyncKeyState(VK_UP))
			{
				if (dos->IncDecDealer(lef))
				{
					LineErase(Line[0], Line[1]);
					Line[1] -= 4;

					LinePrint(Line[0], Line[1]);
				}
			}
			else if (GetAsyncKeyState(VK_DOWN))
			{
				if (dos->IncDecDealer(rig))
				{
					LineErase(Line[0], Line[1]);
					Line[1] += 4;
					LinePrint(Line[0], Line[1]);
				}
			}
			if (GetAsyncKeyState(VK_SPACE))
			{
				FoldOrFilSelect = OptionShowerByArray(FileFolder, Line[0] + 20, Line[1] );
			}
			if (GetAsyncKeyState(0x44))
			{
				MoveToChild();
			}
			if (GetAsyncKeyState(0x4d))
			{
				menuSelector = OptionShowerByArrayUpDown(menuMain, 120, 2);
			}
			if (GetAsyncKeyState(0x57))
			{
				MoveToFather();
			}
			if (GetAsyncKeyState(VK_SHIFT))
			{
				copyState = (!copyState);
			}
			if (this->terminater == true)
			{
				return;
			}
			OptionViewer();
			dos->SetPresentFolder();
			DosOperationManager();
			Viewer();
			Sleep(200);
		}
	}
	// format
	void format()
	{
		dos->SetPresentFolder();
		dos->PresentFolder->FilesInFolder.clear();
		dos->PresentFolder->FoldersInFolders.clear();
		gotoxy(25, 25);
		cout << "FORMAT IS DONE";
		Sleep(2000);
	}
	// dir
	void dir()
	{

		system("cls");
		dos->SetPresentFolder();
		int x = 1;
		int y = 1;
		for (auto i : dos->PresentFolder->FilesInFolder)
		{
			gotoxy(x, y);
			cout << i->Name << "  " << i->path << "    " << i->size << "  " << i->timet;
			y++;
		}
		for (auto i : dos->PresentFolder->FoldersInFolders)
		{
			gotoxy(x, y);
			cout << i->Name << "  " << i->path << "   " << i->timet;
			y++;
		}
		stoper();
	}
	// used to stop or like getch
	void stoper()
	{
		cout << endl;
		string aa;
		cin >> aa;
	}
	// rename
	void Rename()
	{
		string name = toGetAnyInput(40, 40, "ENTER THE NEW NAME: ");
		dos->SetPresentFolder();
		dos->PresentFolder->Name = name;
		print(10, 10);
	}
	// attrib
	void attrib()
	{
		system("cls");
		dos->SetPresentFolder();
		int x = 120;
		int y = 2;
		if (dos->PresentFolder->FilesInFolder.empty() == false)
		{
			for (auto i : dos->PresentFolder->FilesInFolder)
			{
				gotoxy(x, y);
				cout <<"File        " << i->path;
				y++;
			}
		}
		if (dos->PresentFolder->FoldersInFolders.empty() == false)
		{
			for (auto i : dos->PresentFolder->FoldersInFolders)
			{
				gotoxy(x, y);
				cout << "Folder        " << i->path;
				y++;
			}
		}
		string op;
		cin >> op;
	}
	// Delete
	void Delete(string name) {
		int x = 0;
		for (auto i : dos->Present)
		{

			if (i->Name == name)
			{
				dos->Present.erase(dos->Present.begin() + x);
			}
			x++;
		}
	}
	// OptionViewr
	void OptionViewer()
	{
		if (FoldOrFilSelect == 0)
		{
			int x = 140;
			int y = 8;
			int hor = x;
			int ver = y;
			int Li[2] = { x,y + 2 };
			for (auto i : dos->PresentFolder->FoldersInFolders)
			{
				gotoxy(x, y);
				cout << i->Name;
				y += 4;
			}
			int s = 0;
			while (true)
			{
				Sleep(100);
				if (GetAsyncKeyState(VK_UP))
				{
					if (s > 0)
					{
						s--;
						LineErase(Li[0], Li[1]);
						Li[1] -= 4;
						LinePrint(Li[0], Li[1]);
						cout << s;
					}
				}
				if (GetAsyncKeyState(VK_DOWN))
				{
					if (s < dos->PresentFolder->FoldersInFolders.size() - 1)
					{
						s++;
						LineErase(Li[0], Li[1]);
						Li[1] += 4;
						LinePrint(Li[0], Li[1]);
						cout << s;
					}
				}
				if (GetAsyncKeyState(VK_SPACE))
				{
					CopiedOrCut.push_back(dos->PresentFolder->FoldersInFolders[s]);
				}

				if (GetAsyncKeyState(VK_ESCAPE))
				{

					print(10, 10);
					return;
				}
			}
		}
		else if (FoldOrFilSelect == 1)
		{
			int x = 140;
			int y = 10;
			int hor = x;
			int ver = y;
			int Li[2] = { x,y + 2 };
			for (auto i : dos->PresentFolder->FilesInFolder)
			{
				gotoxy(x, y);
				cout << i->Name;
				y += 4;
			}
			int s = 0;
			while (true)
			{
				Sleep(100);
				if (GetAsyncKeyState(VK_UP))
				{
					if (s > 0)
					{
						s--;
						LineErase(Li[0], Li[1]);
						Li[1] -= 4;
						LinePrint(Li[0], Li[1]);
						cout << s;
					}
				}
				if (GetAsyncKeyState(VK_DOWN))
				{
					if (s < dos->PresentFolder->FilesInFolder.size() - 1)
					{
						s++;
						LineErase(Li[0], Li[1]);
						Li[1] += 4;
						LinePrint(Li[0], Li[1]);
						cout << s;
					}
				}
				if (GetAsyncKeyState(VK_SPACE))
				{
					CopiedOrCut.push_back(dos->PresentFolder->FilesInFolder[s]);
				}
				// to open the note pad;
				if (GetAsyncKeyState(0x4d))
				{
					FrontEnd* f = new FrontEnd(dos->PresentFolder->FilesInFolder[s]->data + '\n');
					dos->PresentFolder->FilesInFolder[s]->data = f->Driver();
				}
				if (GetAsyncKeyState(VK_ESCAPE))
				{
					return;
				}
			}
		}
	}
	// paste function
	void paste()
	{
		for (auto i : CopiedOrCut)
		{
			if (i->Type = folder)
			{
				Folder* f = dynamic_cast<Folder*>(i);
				Folder* fold = new Folder(0, i->Name, dos->PresentFolder->path + i->Name);
				fold->Father = dos->PresentFolder;
				fold->FilesInFolder = f->FilesInFolder;
				fold->FoldersInFolders = f->FoldersInFolders;
				dos->PresentFolder->FoldersInFolders.push_back(fold);
			}
			else
			{
				File* f = dynamic_cast<File*>(i);
				File* fold = new File(0, i->Name, dos->PresentFolder->path + i->Name);
				fold->Father = dos->PresentFolder;
				fold->data = f->data;
				dos->PresentFolder->FilesInFolder.push_back(fold);
			}
		}
		CopiedOrCut.clear();
	}
	// add in Que
	void addInQue()
	{
		string x;
		dos->SetPresentFolder();
		filePrinter(dos->PresentFolder->FilesInFolder);
		while (true)
		{
			gotoxy(5, 5);
			cout << "SELECT: ";
			gotoxy(5, 6);
			cin >> x;

			if (x == "exit")
			{
				return;
			}
			else
			{
				int t = stoi(x);
				fileToPrint.push(dos->PresentFolder->FilesInFolder[t]);
			}
		}
	}
	// Files Printe
	void filePrinter(vector<File*> data)
	{
		int c = 0;
		system("cls");
		for (auto i : data)
		{
			gotoxy(10, c + 3);
			cout << c << ". " << i->Name;
			c++;
		}
	}
	// File or Folder Viewer
	void Viewer()
	{

		if (FoldOrFilSelect == 0)
		{
			dos->SetPresentFolder();
			system("cls");
			viewFolder(dos->PresentFolder->FoldersInFolders);
			FoldOrFilSelect = -1;
			print(10, 10);

		}
		else if (FoldOrFilSelect == 1)
		{
			dos->SetPresentFolder();
			system("cls");
			viewFiles(dos->PresentFolder->FilesInFolder);
			FoldOrFilSelect = -1;
			print(10, 10);
		}
	}
	// Move to Father
	void MoveToFather()
	{
		dos->moveTowardFather();
		Line[0] = 10;
		Line[1] = 12;
		print(10, 10);
	}
	//  Move to Sub
	void MoveToChild()
	{
		if (dos->PresentFolder->FoldersInFolders.size() != 0)
		{
			dos->FolderSetter();
			Line[0] = 10;
			Line[1] = 12;

			print(10, 10);
		}
		else
		{
			gotoxy(140, 5);
			cout << "THIS FOLDER IS EMPTY";
		}

	}
	// View Folders
	void viewFolder(vector<Folder*> folders)
	{
		int y = 1;
		for (auto f : folders)
		{
			gotoxy(140, y + 9);
			cout << y << ": " << f->Name;
			cout << "   Parent is: " << f->Father->Name;
			y++;
		}
	}
	// Option Getter
	int Select(int size)
	{
		while (true)
		{
			int x;
			gotoxy(30, 5);
			cout << "SELECT THE THING (0 to Exit): ";
			gotoxy(30 + 33, 5);
			cin >> x;
			if (x > 0 && x <= size)
			{
				return x;
			}
			if (x == 0)
			{
				return 0;
			}
		}
		return -1;

	}
	// View Files
	void viewFiles(vector<File*> Files)
	{
		int y = 1;
		for (auto f : Files)
		{
			gotoxy(140, y + 9);
			cout << y << ": " << f->Name;
			y++;
		}
	}
	// File Operations aLL
	void DosOperationManager()
	{
		if (menuSelector == 0)
		{
			dos->SetPresentFolder();
			File* f = CreateFileInterFace(dos->PresentFolder);
			dos->InsertFile(f);
			menuSelector = -1;
			system("CLS");
			print(10, 10);
		}
		else if (menuSelector == 1)
		{
			dos->SetPresentFolder();
			Folder* f = CreateFolderInterFace(dos->PresentFolder);
			dos->InsertFolder(f);
			menuSelector = -1;
			system("CLS");
			print(10, 10);
		}
		else if (menuSelector == 2)
		{
			Delete(toGetAnyInput(40, 40, "ENTER THE NAME: "));
			menuSelector = -1;
			system("CLS");
			print(10, 10);

		}
		else if (menuSelector == 3)
		{
			Rename();
			menuSelector = -1;
			system("CLS");
			print(10, 10);
		}
		else if (menuSelector == 6)
		{
			paste();
			menuSelector = -1;
			system("CLS");
			print(10, 10);
		}
		else if (menuSelector == 7)
		{
			TreePrinter();
			menuSelector = -1;
		}
		else if (menuSelector == 8)
		{
			Find(toGetAnyInput(40, 40, "ENTER THE NAME: "));
			getchar();
			menuSelector = -1;
			system("CLS");
			print(10, 10);
		}
		else if (menuSelector == 9)
		{
			FindStr(toGetAnyInput(40, 40, "ENTER THE String: "));
			getchar();
			menuSelector = -1;
			system("CLS");
			print(10, 10);
		}
		else if (menuSelector == 10)
		{
			PQueue();
			getchar();
			menuSelector = -1;
			system("CLS");
			print(10, 10);
		}
		else if (menuSelector == 11)
		{
			PWD();
			getchar();
			menuSelector = -1;
			system("CLS");
			print(10, 10);
		}
		else if (menuSelector == 12)
		{
			attrib();
			getchar();
			menuSelector = -1;
			system("CLS");
			print(10, 10);
		}
		else if (menuSelector == 13)
		{
			dos->Present = dos->Disk;
			dos->level = 0;
			Line[0] = 10;
			Line[1] = 12;
			system("CLS");
			print(10, 10);
			menuSelector = -1;
		}
		else if (menuSelector == 14)
		{	
			dir();
			system("CLS");
			print(10, 10);
			menuSelector = -1;
		}
		else if (menuSelector == 15)
		{
			terminater = true;
			menuSelector = -1;
		}
		else if (menuSelector == 16)
		{
			format();
			system("CLS");
			print(10, 10);
			menuSelector = -1;
		}
		else if (menuSelector == 17)
		{
			help();
			system("CLS");
			print(10, 10);
			menuSelector = -1;
		}
		else if (menuSelector == 18)
		{
			ver();
			system("CLS");
			print(10, 10);
			menuSelector = -1;
		}
		else if (menuSelector == 19)
		{
			addInQue();
			system("CLS");
			print(10, 10);
			menuSelector = -1;
		}
		else if (menuSelector == 20)
		{
			Queue();
			system("CLS");
			print(10, 10);
			menuSelector = -1;
		}
	}
	//print Que
	void Queue()
	{
		system("cls");
		int x = 1;
		while (!fileToPrint.empty())
		{
			cout << x <<" " << fileToPrint.front()->Name << endl;
			fileToPrint.pop();
			x++;
		}
		stoper();
	}
	// Print
	void print(int x, int y)
	{
		system("cls");
		header();
		for (auto fol : dos->Present)
		{
			gotoxy(x, y);
			cout << fol->Name << endl;
			y += 4;
		}
	}
	// Line Print
	void LinePrint(int x, int y)
	{
		gotoxy(x, y);
		cout <<"\033[34m" << "############" << "\033[36m";
	}
	// Line Erase
	void LineErase(int x, int y)
	{
		gotoxy(x, y);
		cout << "            ";
	}
	// Show Option by right and left selection
	int OptionShowerByArray(vector<string> arr, int x, int y)
	{
		int hor = x;
		int ver = y;
		int Li[2] = { x,y + 3 };
		for (int i = 0; i < arr.size(); i++)
		{
			gotoxy(x, y);
			cout << i + 1 << ": " << arr[i];
			x += 20;

		}
		int s = 0;
		while (true)
		{
			Sleep(100);
			if (GetAsyncKeyState(VK_LEFT))
			{
				if (s > 0)
				{
					s--;
					LineErase(Li[0], Li[1]);
					Li[0] -= 20;
					LinePrint(Li[0], Li[1]);
				}
			}
			if (GetAsyncKeyState(VK_RIGHT))
			{
				if (s < arr.size() - 1)
				{
					s++;
					LineErase(Li[0], Li[1]);
					Li[0] += 20;
					LinePrint(Li[0], Li[1]);
				}
			}
			if (GetAsyncKeyState(VK_SPACE))
			{
				return s;
			}

			if (GetAsyncKeyState(VK_ESCAPE))
			{
				system("CLS");
				print(10, 10);
				return -1;
			}
		}
		return 0;
	}
	// Show Option by up and down selection
	int OptionShowerByArrayUpDown(vector<string> arr, int x, int y)
	{
		int hor = x;
		int ver = y;
		int Li[2] = { x,y + 1 };
		for (int i = 0; i < arr.size(); i++)
		{
			gotoxy(x, y);
			cout << i + 1 << ": " << arr[i];
			y += 2;

		}
		int s = 0;
		while (true)
		{
			Sleep(100);
			if (GetAsyncKeyState(VK_UP))
			{
				if (s > 0)
				{
					s--;
					LineErase(Li[0], Li[1]);
					Li[1] -= 2;
					LinePrint(Li[0], Li[1]);
					cout << s;
				}
			}
			if (GetAsyncKeyState(VK_DOWN))
			{
				if (s < arr.size() - 1)
				{
					s++;
					LineErase(Li[0], Li[1]);
					Li[1] += 2;
					LinePrint(Li[0], Li[1]);
					cout << s;
				}
			}
			if (GetAsyncKeyState(VK_SPACE))
			{
				return s;
			}

			if (GetAsyncKeyState(VK_ESCAPE))
			{
				system("CLS");
				print(10, 10);
				return -1;
			}
		}
		return 0;
	}
	// To Get Any Input
	string toGetAnyInput(int x, int y, string show)
	{
		string input = "";
		gotoxy(x, y);
		cout << show;
		gotoxy(x + show.size(), y);
		getline(cin, input);
		return input;
	}
	// Create File InterFace
	File* CreateFileInterFace(Folder* F)
	{
		string name = toGetAnyInput(30, 40, "NAME OF FILE: ");
		string data = toGetAnyInput(30, 42, "DATA IN FILE: ");
		File* f = new File(data.size(), name + ".txt", F->path + name,data);
		return f;
	}
	// Create Folder Interface
	Folder* CreateFolderInterFace(Folder* F)
	{
		string name = toGetAnyInput(30, 40, "NAME OF FOLDER: ");
		Folder* f = new Folder(0, name, F->path + name);
		return f;
	}
	//Gotoxy
	void gotoxy(int x, int y)
	{
		COORD coordinates;
		coordinates.X = x;
		coordinates.Y = y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinates);
	}
	// TreePrinter()
	void TreePrinter()
	{
		system("cls");
		gotoxy(0, 3);
		dos->SetPresentFolder();
		gotoxy(0, 0);
		cout << dos->PresentFolder->Name;
		int y = 1;
		TreePrinter(dos->PresentFolder->FoldersInFolders, 1, y,10);
		Sleep(5000);
		stoper();
		print(10, 10);
	}
	// tree printer with parameters
	void TreePrinter(vector<Folder*> folders, int num,int &y,int x)
	{
	
		for (int i = 0; i < folders.size(); i++)
		{
			gotoxy(x, y);
			cout << "|_____" << folders[i]->Name << endl;
			if (!folders[i]->FoldersInFolders.empty())
			{
				y++;
				TreePrinter(folders[i]->FoldersInFolders, num + 3, y,x+5);
			}
			y++;
			
		}
	}
	// find only
	void Find(string name)
	{
		gotoxy(0, 1);
		system("CLS");
		dos->SetPresentFolder();
		int x = 0;
		int y = 10;
		for (const auto& fold : dos->PresentFolder->FoldersInFolders)
		{
			if (fold->Name.substr(0, name.length()) == name)
			{
				gotoxy(10, x);
				cout << x + 1 <<": " << fold->Name;
				x++;
			}
		}
		for (const auto& fold : dos->PresentFolder->FilesInFolder)
		{
			if (fold->Name.substr(0, name.length()) == name)
			{
				gotoxy(10, x);
				cout << x + 1 <<": " << fold->Name;
				x++;
			}
		}
	}
	// find in files
	void FindStr(string name)
	{

		gotoxy(0, 1);
		system("CLS");
		dos->SetPresentFolder();
		int x = 0;
		int y = 10;
		for (const auto& fold : dos->PresentFolder->FilesInFolder)
		{
			int x = 0;
			size_t pos = fold->Name.find(name);
			if (pos != std::string::npos)
			{
				gotoxy(10, x);
				cout << "File: " << fold->Name;
				temp.push_back(fold);
				x++;
			}
		}
	}
	// Print in Pque
	void PQueue()
	{
		gotoxy(0, 1);
		system("CLS");
		dos->SetPresentFolder();
		int x = 0;
		priority_queue<File*> fileQueue;
		for (auto& f : dos->PresentFolder->FilesInFolder)
		{
			fileQueue.push(f);
		}
		while (!fileQueue.empty()) {
			File* currentFile = fileQueue.top();
			cout << "Name: " << currentFile->Name << ", Size: " << currentFile->size << endl;
			fileQueue.pop();
		}
	}
	// PWD
	void PWD()
	{
		gotoxy(5, 5);
		dos->SetPresentFolder();
		cout << "Name: " << dos->PresentFolder->Name << " Path:" << dos->PresentFolder->path;
	}
	// header printer
	void header()
	{

		gotoxy(40, 1);
		cout  << "\033[32m" << " ____  __   __  ____  __" << "\033[33m"<<"    ____   __   ____";
		gotoxy(40, 2);
		cout << "(  __)/ _\\ (  )(__  )(  "<<"\033[34m"<<")  (    \\ /  \\ / ___)";
		gotoxy(40, 3);
		cout << " ) _)/    " << "\033[38m" << "\\ )(  / _/  )(    " << "\033[32m" << ") D ((  O )\\___ \\";
		gotoxy(40, 4);
		cout <<"\033[32m"<< "(__) \\_/\\_/(__)(____)" << "\033[31m" << "(__)  (____/ \\__/ (____/" << "\033[32m";


	}
	// Help
	void help()
	{
		system("cls");
		string commandDescriptions[] = {
		"ADD FILE: Adds a new file to the current directory.",
		"ADD FOLDER: Creates a new folder (directory) in the current directory.",
		"DEL PRESENT: Deletes the present (current) file or folder.",
		"RENAME PRESENT: Renames the present (current) file or folder.",
		"COPY: Copies a file or folder to another location.",
		"CUT: Cuts a file or folder for moving to another location.",
		"PAST: Pastes the previously cut or copied file or folder to the current location.",
		"Tree Printer: Prints a hierarchical tree structure of the files and folders in the current directory.",
		"Find: Searches for a file or folder in the current directory.",
		"FINDSTR: Searches for a specific string or pattern in files.",
		"PQUEUE: Manages a priority queue (specific functionality may vary).",
		"PWD: Prints the current working directory (shows the full path of the current location).",
		"ATTRIB: Displays or changes file attributes (such as read-only, hidden, etc.).",
		"CD SLASH: Changes the current directory to the root directory.",
		"DIR: Lists the files and folders in the current directory.",
		"EXIT: Exits or closes the command line interface.",
		"FORMATE: Formats a storage device (e.g., a disk or drive).",
		"HELP: Displays help information for the command line interface or a specific command."
		};
		gotoxy(0, 5);
		cout << "HELP";
		gotoxy(0, 10);
		// Printing the commands and their descriptions
		for (const auto& commandDescription : commandDescriptions) {
			
			cout << commandDescription << endl;
		}
		stoper();

	}
	// version
	void ver()
	{
		gotoxy(25, 25);
		cout << "Faizi Dos [Version: 12:19:2023:6:33]";
		stoper();
	}
};

int main()
{
	DosFrontEnd d;
	d.DosWorkingAll();
}
