#include<iostream>
#include<ctime>
#include<ctime>
#include<vector>
#include<string>
#include<windows.h>
using namespace std;
class Folder;

class BaseOfAll {
public:
	int size;
	string Name;
	string positonByDate;
	string path;
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
	void SetFather(Folder *Father)
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

	string data;
public:
	File(int size, string Name, string path,string data)
		: BaseOfAll(size, Name, path)
	{
		this->data = data;
	}
	File(int size, string Name, string path)
		: BaseOfAll(size, Name, path)
	{

		this->data = "";
	}
};

class Folder : public BaseOfAll {
public:
	vector<Folder*> FoldersInFolders;
	vector<File*> FilesInFolder;
	Folder(int size, string Name, string path)
		: BaseOfAll(size, Name, path)
	{

	}
};



class Dos {
public:
	vector<Folder*> Disk;
	Folder* Present;
	Dos(int n)
	{
		CreateDisk(n);
	}

	void InsertFile(File *F,Folder *Father)
	{
		F->Father = Father;
		Father->FilesInFolder.push_back(F);
	}
	void InsertFolder(Folder *F,Folder *ChangeHappen)
	{

	}
	void CreateDisk(int size)
	{
		string name = "Local Disk ";
		char na = 'A';
		for (int i = 0; i < size; i++)
		{
			Folder *fl = new Folder(0, name + na, name + na + '/');
			na++;
			Disk.push_back(fl);
		}
	}

	void print()
	{
		for (auto fol : Disk)
		{
			cout << fol->Name<<endl;
		}
	}

};

class DosFrontEnd {
Dos* dos;
public:
	DosFrontEnd()
	{
		dos = new Dos(stoi(toGetAnyInput(10, 10, "Enter: ")));

	}
	string toGetAnyInput(int x,int y,string show)
	{
		string input = "";
		gotoxy(x, y);
		cout << show;
		gotoxy(x + show.size(), y);
		cin>>input;
		return input;
	}
	File* CreateFileInterFace(Folder *F)
	{
		string name = toGetAnyInput(10, 10, "NAME: ");
		string data = toGetAnyInput(10, 12, "DATA: ");
		File* f = new File(data.size(),name,F->path + name);
		return f;
	}
	void gotoxy(int x, int y)
	{
		COORD coordinates;
		coordinates.X = x;
		coordinates.Y = y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinates);
	}
};

int main()
{
	DosFrontEnd d;
	as->print();
	as->InsertFile(d.CreateFileInterFace(as->Disk[0]), as->Disk[0]);
	cout << as->Disk[0]->FilesInFolder[0];
	
}