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
		if (path[0] == '/') // ���� ���
		{
			return find_impl(root, path.substr(1));
		}
		else // ��� ���
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
			cout << current->name << "��(��) �����Դϴ�." << endl;
			return false;
		}

		auto sep = path.find('/');
		
		// path�� '/'�� ���� ���
		if (sep == string::npos)
		{
			auto found = find_if(current->children.begin(), current->children.end(), [&](const node* child) {
				return child->name == path;
				});

			if (found != current->children.end())
			{
				cout << current->name << "�� �̹� " << path << " �̸��� ����/���͸��� �ֽ��ϴ�." << endl;
				return false;
			}

			current->children.push_back(new node{ path, flag, {} });
			return true;
		}

		// path�� '/'�� �ִ� ���, ��, ���͸� �̸��� �����ϰ� �ִ� ���
		string next_dir = path.substr(0, sep);
		auto found = find_if(current->children.begin(), current->children.end(), [&](const node* child) {
			return child->name == next_dir && child->flag;
			});

		if (found != current->children.end())
		{
			return add_impl(*found, path.substr(sep + 1), flag);
		}

		// path�� ���͸� �̸��� ���ԵǾ� ������, �ش� ���͸��� ���� ���
		cout << current->name << "�� " << next_dir << " �̸��� ���͸��� �����ϴ�." << endl;
		return false;
	}
public:
	void show_path(const string& path)
	{
		auto found = find(path);
		if (not found)
		{
			cout << path << " ��ΰ� �������� �ʽ��ϴ�." << endl;
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
			cout << "���� ���͸��� " << cur_dir->name << "�� �̵��մϴ�." << endl;
			return true;
		}
		cout << path << " ��θ� ã�� �� �����ϴ�." << endl;
		return false;
	}
};

int main()
{
	file_system fs;
	fs.add("usr", true); // "/"�� usr ���͸� �߰�
	fs.add("etc", true); // "/"�� etc ���͸� �߰�
	fs.add("var", true); // "/"�� var ���͸� �߰�
	fs.add("tmp_file", false); // "/"�� tmp_file ���� �߰�

	cout << "\"/\"�� ����/���͸� ���:" << endl;
	fs.show_path("/"); // "/"�� ����/���͸� ��� ���

	cout << endl;
	fs.change_dir("usr");
	fs.add("gilbut", true);
	fs.add("gilbut/Downloads", true);
	fs.add("gilbut/Downloads/newFile.cpp", false);

	cout << "���� ���͸����� usr�� ����/���͸� ���: " << endl;
	fs.show_path("usr"); // ���� ���͸����� usr ���͸��� �����Ƿ� ���������� ������� ����

	cout << "\"/usr\"�� ����/���͸� ���:" << endl;
	fs.show_path("/usr");

	cout << "\"/usr/gilbut/Downloads\"�� ����/���͸� ���:" << endl;
	fs.show_path("/usr/gilbut/Downloads");
}