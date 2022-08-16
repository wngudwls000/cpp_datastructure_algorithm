#include <string>
#include <vector>
#include <iostream>
#include <set>
#include <map>
#include <stack>

using namespace std;

template <typename T>
struct Edge
{
	unsigned src;
	unsigned dst;
	T weight;
};

template <typename T>
class Graph
{
public:
	Graph(unsigned N) : V(N) {}

	auto vertices() const { return V; }

	auto& edges() const { return edge_list; }

	auto edges(unsigned v) const
	{
		vector<Edge<T>> edges_from_v;
		for (auto& e : edge_list)
		{
			if (e.src == v)
				edges_from_v.emplace_back(e);
		}

		return edges_from_v;
	}

	void add_edge(Edge<T>&& e)
	{
		if (e.src >= 1 && e.src <= V && e.dst >= 1 && e.dst <= V)
			edge_list.emplace_back(e);
		else
			cerr << "����: ��ȿ ������ ��� ����!" << endl;
	}

	template <typename U>
	friend ostream& operator<< (ostream& os, const Graph<U>& G);

private:
	unsigned V;
	vector<Edge<T>> edge_list;
};

template <typename U>
ostream& operator<<(ostream& os, const Graph<U>& G)
{
	for (unsigned i = 1; i < G.vertices(); i++)
	{
		os << i << ":\t";

		auto edges = G.edges(i);
		for (auto& e : edges)
			os << "{" << e.dst << ": " << e.weight << "}.";

		os << endl;
	}

	return os;
}

template <typename T>
auto create_bipartite_reference_graph()
{
	Graph<T> G(10);

	map<unsigned, vector<pair<unsigned, T>>> edge_map;
	edge_map[1] = { {2,0} };
	edge_map[2] = { {1,0},{3,0},{8,0} };
	edge_map[3] = { {2,0},{4,0} };
	edge_map[4] = { {3,0},{6,0} };
	edge_map[5] = { {7,0},{9,0} };
	edge_map[6] = { {4,0}};
	edge_map[7] = { {5,0} };
	edge_map[8] = { {2,0},{9,0} };
	edge_map[9] = { {5,0},{8,0} };

	for (auto& i : edge_map)
		for (auto& j : i.second)
			G.add_edge(Edge<T>{i.first, j.first, j.second});

	return G;
}

template <typename T>
auto bipartite_check(const Graph<T>& G)
{
	stack<unsigned> stack;
	set<unsigned> visited; // �湮�� ����
	stack.push(1);

	enum class colors { NONE, BLACK, RED };
	colors current_color{ colors::BLACK }; // ���� ������ ĥ�� ����

	vector<colors> vertex_colors(G.vertices(), colors::NONE);

	while (!stack.empty())
	{
		auto current_vertex = stack.top();
		stack.pop();

		// ���� ������ ������ �湮���� �ʾҴٸ�
		if (visited.find(current_vertex) == visited.end())
		{
			visited.insert(current_vertex);
			vertex_colors[current_vertex] = current_color;

			if (current_color == colors::RED)
			{
				cout << current_vertex << "�� ����: ������" << endl;
				current_color = colors::BLACK;
			}
			else
			{
				cout << current_vertex << "�� ����: ������" << endl;
				current_color = colors::RED;
			}

			for (auto& e : G.edges(current_vertex))
			{
				// ������ ���� �߿��� �湮���� ���� ������ �ִٸ� ���ÿ� �߰�
				if (visited.find(e.dst) == visited.end())
				{
					stack.push(e.dst);
				}
			}
		}
		// ���� ������ �̹� �湮�� �����̰�, ���� ĥ�� ����� ���� ������ ĥ���� �ִٸ� �̺� �׷����� �ƴ�
		else if (vertex_colors[current_vertex] != colors::NONE && vertex_colors[current_vertex] != current_color)
			return false;
	}

	return true;
}

int main()
{
	using T = unsigned;

	// �׷��� ��ü ����
	auto BG = create_bipartite_reference_graph<T>();
	cout << "[�Է� �׷���]" << endl;
	cout << BG << endl;

	if (bipartite_check<T>(BG))
		cout << endl << "�̺� �׷����� �½��ϴ�." << endl;
	else
		cout << endl << "�̺� �׷����� �ƴմϴ�." << endl;
}