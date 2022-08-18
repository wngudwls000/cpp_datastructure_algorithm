#include <vector>
#include <iostream>
#include <climits>


using namespace std;

struct Edge
{
	int src;
	int dst;
	int weight;
};

const int UNKNOWN = INT_MAX;

vector<int> BellmanFord(vector<Edge> edges, int V, int start)
{
	vector<int> distance(V, UNKNOWN);
	distance[start] = 0;

	// (V -1)�� �ݺ�
	for (int i = 0; i < V - 1; i++)
	{
		// ��ü ������ ���� �ݺ�
		for (auto& e : edges)
		{
			// ������ ���� ������ �Ÿ� ���� UNKNOWN�̸� ��ŵ
			if (distance[e.src] == UNKNOWN)
				continue;

			// ������ ������ �Ÿ� ���� ���ο� ��ο� ���� �Ÿ����� ũ�� �Ÿ� ���� ������Ʈ��
			if (distance[e.dst] > distance[e.src] + e.weight)
			{
				distance[e.dst] = distance[e.src] + e.weight;
			}
		}
	}

	return distance;
}

int main()
{
	int V = 5; // ���� ����
	vector<Edge> edges; // ���� �������� ����

	vector<vector<int>> edge_map{ // {���� ����, ��ǥ ����, ����ġ}
		{0, 1, 3},
		{1, 2, 5},
		{1, 3, 10},
		{3, 2, -7},
		{2, 4, 2}
	};

	for (auto& e : edge_map)
	{
		edges.emplace_back(Edge{ e[0], e[1], e[2] });
	}

	int start = 0;
	vector<int> distance = BellmanFord(edges, V, start);

	cout << "[" << start << "�� �������κ��� �ּ� �Ÿ�]" << endl;

	for (int i = 0; i < distance.size(); i++)
	{
		if (distance[i] == UNKNOWN)
			cout << i << "�� ����: �湮���� ����!" << endl;
		else
			cout << i << "�� ����: " << distance[i] << endl;
	}
}

