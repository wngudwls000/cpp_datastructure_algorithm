#include <iostream>
#include <vector>
#include <algorithm>

#define DIRECTORY true
#define FILE false

using namespace std;

struct node
{
	string name;
	bool flag;
	vector<node*> children;
};

struct file_system
{
private:
	node* root;
	node* cur_dir;
public:
	file_system ()
	{
		root = new node{ "/", DIRECTORY, {} };
		cur_dir = root;
	}

	node* find(const string& path)
	{
		if (path[0] == '/') // 절대 경로
		{
			return find_impl(root, path.substr(1));
		}
		else // 상대 경로
		{
			return find_impl(cur_dir, path);
		}
	}
private:
	node* find_impl(node* current, const string& path)
	{
		if (path.empty())
		{
			return current;
		}

		auto sep = path.find('/');
		string current_path = sep == string::npos ? path : path.substr(0, sep);
		string rest_path = sep == string::npos ? "" : path.substr(sep + 1);

		auto found = find_if(current->children.begin(), current->children.end(), [&](const node* child) {
			return child->name == current_path;
			});
		if (found != current->children.end())
		{
			return find_impl(*found, rest_path);
		}
		return NULL;
	}

public:
	bool add(const string& path, bool flag)
	{
		if (path[0] == '/')
		{
			return add_impl(root, path.substr(1), flag);
		}
		else
		{
			return add_impl(cur_dir, path, flag);
		}
	}
private:
	bool add_impl(node* current, const string& path, bool flag)
	{
		if (not current->flag)
		{
			cout << current->name << "은(는) 파일입니다." << endl;
			return false;
		}

		auto sep = path.find('/');
		
		// path에 '/'가 없는 경우
		if (sep == string::npos)
		{
			auto found = find_if(current->children.begin(), current->children.end(), [&](const node* child) {
				return child->name == path;
				});

			if (found != current->children.end())
			{
				cout << current->name << "에 이미 " << path << " 이름의 파일/디렉터리가 있습니다." << endl;
				return false;
			}

			current->children.push_back(new node{ path, flag, {} });
			return true;
		}

		// path에 '/'가 있는 경우, 즉, 디렉터리 이름을 포함하고 있는 경우
		string next_dir = path.substr(0, sep);
		auto found = find_if(current->children.begin(), current->children.end(), [&](const node* child) {
			return child->name == next_dir && child->flag;
			});

		if (found != current->children.end())
		{
			return add_impl(*found, path.substr(sep + 1), flag);
		}

		// path에 디렉터리 이름이 포함되어 있지만, 해당 디렉터리가 없는 경우
		cout << current->name << "에 " << next_dir << " 이름의 디렉터리가 없습니다." << endl;
		return false;
	}
public:
	void show_path(const string& path)
	{
		auto found = find(path);
		if (not found)
		{
			cout << path << " 경로가 존재하지 않습니다." << endl;
			return;
		}

		if (found->flag)
		{
			for (auto child : found->children)
			{
				cout << (child->flag ? "d " : "- ") << child->name << endl;
			}
		}
		else
		{
			cout << "- " << found->name << endl;
		}
	}

	bool change_dir(const string& path)
	{
		auto found = find(path);
		if (found && found->flag)
		{
			cur_dir = found;
			cout << "현재 디렉터리를 " << cur_dir->name << "로 이동합니다." << endl;
			return true;
		}
		cout << path << " 경로를 찾을 수 없습니다." << endl;
		return false;
	}
};

int main()
{
	file_system fs;
	fs.add("usr", true); // "/"에 usr 디렉터리 추가
	fs.add("etc", true); // "/"에 etc 디렉터리 추가
	fs.add("var", true); // "/"에 var 디렉터리 추가
	fs.add("tmp_file", false); // "/"에 tmp_file 파일 추가

	cout << "\"/\"의 파일/디렉터리 목록:" << endl;
	fs.show_path("/"); // "/"의 파일/디렉터리 목록 출력

	cout << endl;
	fs.change_dir("usr");
	fs.add("gilbut", true);
	fs.add("gilbut/Downloads", true);
	fs.add("gilbut/Downloads/newFile.cpp", false);

	cout << "현재 디렉터리에서 usr의 파일/디렉터리 목록: " << endl;
	fs.show_path("usr"); // 현재 디렉터리에는 usr 디렉터리가 없으므로 정상적으로 출력하지 못함

	cout << "\"/usr\"의 파일/디렉터리 목록:" << endl;
	fs.show_path("/usr");

	cout << "\"/usr/gilbut/Downloads\"의 파일/디렉터리 목록:" << endl;
	fs.show_path("/usr/gilbut/Downloads");
}