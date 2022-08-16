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
			std::cout << manager << "을(를) 찾을 수 없습니다: " << std::endl;
			return false;
		}

		if (managerNode->first_sub && managerNode->second_sub)
		{
			std::cout << manager << " 아래에 " << subordinate << "을(를) 추가할 수 없습니다." << std::endl;
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

		std::cout << manager << " 아래에 " << subordinate << "을(를) 추가했습니다." << std::endl;

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

	tree.addSubordinate("CEO", "부사장");
	tree.addSubordinate("부사장", "IT부장");
	tree.addSubordinate("부사장", "마케팅부장");
	tree.addSubordinate("IT부장", "보안팀장");
	tree.addSubordinate("IT부장", "앱개발팀장");
	tree.addSubordinate("마케팅부장", "물류팀장");
	tree.addSubordinate("마케팅부장", "홍보팀장");
	tree.addSubordinate("부사장", "재무부장");
	std::cout << std::endl;
	std::cout << "전위 순회" << std::endl;
	org_tree::preOrder(tree.root);
	std::cout << std::endl << std::endl;
	std::cout << "중위 순회" << std::endl;
	org_tree::inOrder(tree.root);
	std::cout << std::endl << std::endl;
	std::cout << "후위 순회" << std::endl;
	org_tree::postOrder(tree.root);
	std::cout << std::endl << std::endl;
	std::cout << "레벨 순회" << std::endl;
	org_tree::levelOrder(tree.root);
}