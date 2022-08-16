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

	static void preOrder(node* start)
	{
		if (!start)
			return;

		std::cout << start->position << ", ";
		preOrder(start->first_sub);
		preOrder(start->second_sub);
	}
	static void inOrder(node* start)
	{
		if (!start)
			return;

		inOrder(start->first_sub);
		std::cout << start->position << ", ";
		inOrder(start->second_sub);
	}
	static void postOrder(node* start)
	{
		if (!start)
			return;

		postOrder(start->first_sub);
		postOrder(start->second_sub);
		std::cout << start->position << ", ";
	}
	static void levelOrder(node* start)
	{
		if (!start)
			return;

		std::queue<node*> q;
		q.push(start);

		while (!q.empty())
		{
			int size = q.size();
			for (int i = 0; i<size; i++)
			{
				auto current = q.front();
				q.pop();

				std::cout << current->position << ", ";
				if (current->first_sub)
				{
					q.push(current->first_sub);
				}
				if (current->second_sub)
				{
					q.push(current->second_sub);
				}
			}
			std::cout << std::endl;
		}
	}
};

int main()
{
	auto tree = org_tree::create_org_structure("CEO");

	tree.addSubordinate("CEO", "�λ���");
	tree.addSubordinate("�λ���", "IT����");
	tree.addSubordinate("�λ���", "�����ú���");
	tree.addSubordinate("IT����", "��������");
	tree.addSubordinate("IT����", "�۰�������");
	tree.addSubordinate("�����ú���", "��������");
	tree.addSubordinate("�����ú���", "ȫ������");
	tree.addSubordinate("�λ���", "�繫����");
	std::cout << std::endl;
	std::cout << "���� ��ȸ" << std::endl;
	org_tree::preOrder(tree.root);
	std::cout << std::endl << std::endl;
	std::cout << "���� ��ȸ" << std::endl;
	org_tree::inOrder(tree.root);
	std::cout << std::endl << std::endl;
	std::cout << "���� ��ȸ" << std::endl;
	org_tree::postOrder(tree.root);
	std::cout << std::endl << std::endl;
	std::cout << "���� ��ȸ" << std::endl;
	org_tree::levelOrder(tree.root);
}