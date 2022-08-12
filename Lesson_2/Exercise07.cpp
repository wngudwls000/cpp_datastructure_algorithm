#include <iostream>
#include <queue>

struct node
{
	std::string position;
	node* first_sub;
	node* second_sub;
};

struct org_tree
{
	node* root;

	static org_tree create_org_structure(const std::string& pos)
	{
		org_tree tree;
		tree.root = new node{ pos, NULL, NULL };
		return tree;
	}

	static node* find(node* root, const std::string& value)
	{
		if (root == NULL)
		{
			return NULL;
		}

		if (root->position == value)
		{
			return root;
		}

		auto firstfound = org_tree::find(root->first_sub, value);

		if (firstfound != NULL)
		{
			return firstfound;
		}

		return org_tree::find(root->second_sub, value);
	}

	bool addSubordinate(const std::string& manager, const std::string& subordinate)
	{
		auto managerNode = org_tree::find(root, manager);

		if (!managerNode)
		{
			std::cout << manager << "��(��) ã�� �� �����ϴ�: " << std::endl;
			return false;
		}

		if (managerNode->first_sub && managerNode->second_sub)
		{
			std::cout << manager << " �Ʒ��� " << subordinate << "��(��) �߰��� �� �����ϴ�." << std::endl;
			return false;
		}

		if (!managerNode->first_sub)
		{
			managerNode->first_sub = new node{ subordinate, NULL, NULL };
		}
		else
		{
			managerNode->second_sub = new node{ subordinate, NULL, NULL };
		}

		std::cout << manager << " �Ʒ��� " << subordinate << "��(��) �߰��߽��ϴ�." << std::endl;

		return true;
	}
};

int main()
{
	auto tree = org_tree::create_org_structure("CEO");

	tree.addSubordinate("CEO", "�λ���");
	tree.addSubordinate("�λ���", "IT����");
	tree.addSubordinate("�λ���", "�����ú���");
	tree.addSubordinate("IT����", "��������");
	tree.addSubordinate("IT����", "�۰��� ����");
	tree.addSubordinate("�����ú���", "��������");
	tree.addSubordinate("�����ú���", "ȫ������");
	tree.addSubordinate("�λ���", "�繫����");
}