#include<iostream>
#include<ctime>
#include<ctime>
#include<vector>
#include<queue>
#include<string>
#include<windows.h>
#include<string>
using namespace std;
class Folder;

enum position {
	lef, rig, up, down
};
enum What {
	file,folder
};

class BaseOfAll {
public:
	int size;
	string Name;
	string positonByDate;
	string path;
	What Type;
	// true mean it is a txt or any other file
	bool checker;
	Folder* Father;
	BaseOfAll(int size, string Name, string path)
	{
		this->size = size;
		this->Name = Name;
		this->path = path;
		this->checker = ExtensionChecker(this->Name, Name.length());
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
	vector<string> menuMain = { "ADD FILE","ADD FOLDER","DEL PRESENT","RENAME PRESENT","COPY","CUT","PAST","Tree Printer","Find","FINDSTR","PQUEUE","PWD"};
	vector<string> CopyCutOpt = { "ADD FILE","ADD FOLDER","DEL PRESENT","RENAME PRESENT","COPY","CUT" };
	int menuSelector = -1;
	int FoldOrFilSelect = -1;
	vector<BaseOfAll *> CopiedOrCut;
	bool copyState = false;
	Folder* CopyFolder;
	vector<BaseOfAll*> temp;
public:
	DosFrontEnd()
	{
		dos = new Dos(stoi(toGetAnyInput(10, 10, "Enter: ")));
	}
	void DosWorkingAll()
	{
		print(10, 10);
		LinePrint(Line[0], Line[1]);
		while (true)
		{
			gotoxy(4, 4);
			cout << dos->level;
			if (GetAsyncKeyState(VK_LEFT))
			{
				if (dos->IncDecDealer(lef))
				{
					LineErase(Line[0], Line[1]);
					Line[0] -= 20;

					LinePrint(Line[0], Line[1]);
				}
			}
			else if (GetAsyncKeyState(VK_RIGHT))
			{
				if (dos->IncDecDealer(rig))
				{
					LineErase(Line[0], Line[1]);
					Line[0] += 20;
					LinePrint(Line[0], Line[1]);
				}
			}
			if (GetAsyncKeyState(VK_SPACE))
			{
				FoldOrFilSelect = OptionShowerByArray(FileFolder, Line[0] - 3, Line[1] + 3);
			}
			if (GetAsyncKeyState(0x44))
			{
				MoveToChild();
			}
			if (GetAsyncKeyState(0x4d))
			{
				menuSelector = OptionShowerByArrayUpDown(menuMain, 120, 10);
			}
			if (GetAsyncKeyState(0x57))
			{ 
				MoveToFather();
			}
			if (GetAsyncKeyState(VK_SHIFT))
			{
				copyState = (!copyState);
			}
			OptionViewer();
			dos->SetPresentFolder();
			DosOperationManager();
			Viewer();
			Sleep(200);
		}
	}
	void Rename()
	{
		string name = toGetAnyInput(40, 40, "ENTER THE NEW NAME: ");
		dos->SetPresentFolder();
		dos->PresentFolder->Name = name;
		print(10,10);
	}
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
	void OptionViewer()
	{
		if (FoldOrFilSelect == 0)
		{
			int x = 140;
			int y = 10;
			int hor = x;
			int ver = y;
			int Li[2] = { x,y + 2 };
			for (auto i : dos->PresentFolder->FoldersInFolders)
			{
				gotoxy(x, y);
				cout <<i->Name;
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

				if (GetAsyncKeyState(VK_ESCAPE))
				{
					return;
				}
			}
		}
	}
	void paste()
	{
		for (auto i : CopiedOrCut)
		{
			if (i->Type = folder)
			{
				Folder* f = dynamic_cast<Folder*>(i);
				Folder* fold =  new Folder(0,i->Name,dos->PresentFolder->path + i->Name);
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
			gotoxy(140,5);
			cout << "THIS FOLDER IS EMPTY";
		}
		
	}
	// View Folders
	void viewFolder(vector<Folder*> folders)
	{
		int y = 1;
		for (auto f : folders)
		{
			gotoxy(140, y +9);
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
			cout <<y <<": " << f->Name;
			y++;
		}
	}
	// File Operations aLL
	void DosOperationManager()
	{
		if (menuSelector == 0)
		{
			dos->SetPresentFolder();
			File * f = CreateFileInterFace(dos->PresentFolder);
			dos->InsertFile(f);
			menuSelector = -1;
			system("CLS");
			print(10,10);
		}
		else if(menuSelector == 1)
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
	}	
	// Print
	void print(int x, int y)
	{
		system("cls");
		for (auto fol : dos->Present)
		{
			gotoxy(x, y);
			cout << fol->Name << endl;
			x += 20;
		}
	}
	// Line Print
	void LinePrint(int x, int y)
	{
		gotoxy(x, y);
		cout << "############";
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
		int Li[2] = { x,y + 2 };
		for (int i = 0; i < arr.size(); i++)
		{
			gotoxy(x, y);
			cout << i + 1 << ": " << arr[i];
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
				if (s < arr.size() - 1)
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
				return s;
			}

			if (GetAsyncKeyState(VK_ESCAPE))
			{
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
		File* f = new File(data.size(), name+".txt", F->path + name);
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
		TreePrinter(dos->Disk, 1, 1);
		Sleep(5000);
		getchar();
		print(10,10);
	}
	void TreePrinter(vector<Folder*> folders,int num,int y)
	{

		for (int i = 0; i < folders.size(); i++)
		{
			
			cout << "Folder: " << folders[i]->Name << endl;
			if (!folders[i]->FoldersInFolders.empty())
			{
				TreePrinter(folders[i]->FoldersInFolders, num + 3,y++);
			}
			if (!folders[i]->FilesInFolder.empty())
			{
				for (int j = 0; j < folders[j]->FilesInFolder.size(); j++)
				{

					cout << "File: " << folders[i]->FilesInFolder[j]->Name;
				}
			}
		}
	}
	void Find(string name)
	{
		gotoxy(0, 1);
		system("CLS");
		dos->SetPresentFolder();
		int x = 0;
		for (const auto& fold : dos->PresentFolder->FoldersInFolders)
		{
			if (fold->Name.substr(0, name.length()) == name)
			{
				cout << x + 1 << fold->Name;
				x++;
			}		
		}
		for (const auto& fold : dos->PresentFolder->FilesInFolder)
		{
			if (fold->Name.substr(0, name.length()) == name)
			{
				cout << x + 1 << fold->Name;
				x++;
			}
		}
	}
	void FindStr(string name)
	{

		gotoxy(0, 1);
		system("CLS");
		dos->SetPresentFolder();
		int x = 0;
		for (const auto& fold : dos->PresentFolder->FilesInFolder)
		{
			int x = 0;
			size_t pos = fold->Name.find(name);
			if (pos != std::string::npos)
			{
				cout << "File: " << fold->Name;
				temp.push_back(fold);
			}
		}
	}
	void PQueue()
	{
		gotoxy(0, 1);
		system("CLS");
		dos->SetPresentFolder();
		int x = 0;
		priority_queue<File*> fileQueue;
		for (auto& f: dos->PresentFolder->FilesInFolder)
		{
			fileQueue.push(f);
		}
		while (!fileQueue.empty()) {
			File* currentFile = fileQueue.top();
			cout << "Name: " << currentFile->Name << ", Size: " << currentFile->size << endl;
			fileQueue.pop();
		}
	}
	void PWD()
	{
		gotoxy(5, 5);
		dos->SetPresentFolder();
		cout << "Name: " << dos->PresentFolder->Name<<" Path:" << dos->PresentFolder->path;
	}

};

int main()
{
	DosFrontEnd d;
	d.DosWorkingAll();
}
