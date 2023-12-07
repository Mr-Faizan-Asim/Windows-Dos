#include<iostream>
#include<ctime>
#include<ctime>
#include<vector>


using namespace std;

class BaseOfAll {
public:
	int size;
	string Name;
	string positonByDate;
	string path;
	BaseOfAll(int size,string Name,string path)
	{
		this->size = size;
		this->Name = Name;
		this->path = path;

	}

};

class File: public BaseOfAll{
	string data;
public:
	File(int size, string Name, string path)
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

class Folder : public BaseOfAll{
public:
	vector<File> FilesInFolder;
	Folder(int size, string Name, string path)
		: BaseOfAll(size, Name, path)
	{

	}	
};

class Dos {
	
public:

};

int main()
{
	
}