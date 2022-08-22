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

bool HasNegativeCycle(const vector<Edge>& edges, vector<int> distance)
{
	for (auto& e : edges)
	{
		if (distance[e.src] == UNKNOWN)
			continue;


		if (distance[e.src] + e.weight < distance[e.dst])
			return true;
	}
	return false;
}

vector<int> BellmanFord(vector<Edge> edges, int V)
{
	vector<int> distance(V + 1, UNKNOWN);
	int s = V;
	for (int i = 0; i < V; i++)
	{
		edges.push_back(Edge{ s, i, 0 });
	}
	distance[s] = 0;

	// ���� ������ V + 1���̹Ƿ� V�� �ݺ�
	for (int i = 0; i < V; i++)
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

	// ���� ����ġ ����Ŭ�� �ִ��� �˻�
	if (HasNegativeCycle(edges, distance))
	{
		cout << "���� ����ġ ����Ŭ �߰�!" << endl;
		return {};
	}

	return distance;
}

int GetMinDistance(vector<int>& distance, vector<bool>& visited)
{
	int minDistance = UNKNOWN;
	int minIndex = -1;

	for (int i = 0; i < distance.size(); i++)
	{
		if (!visited[i] && distance[i] <= minDistance)
		{
			minDistance = distance[i];
			minIndex = i;
		}
	}

	return minIndex;
}

vector<int> Dijkstra(vector<Edge> edges, int V, int start)
{
	vector<int> distance(V, UNKNOWN);
	vector<bool> visited(V, false);

	distance[start] = 0;

	for (int i = 0; i < V; i++)
	{
		// �湮���� ���� ���� �߿��� �ּ� �Ÿ� ������ ã��
		int curr = GetMinDistance(distance, visited);

		visited[curr] = true;

		for (auto& e : edges)
		{
			// ������ ������ ���
			if (e.src != curr)
				continue;

			// �̹� �湮������ ����
			if (visited[e.dst])
				continue;

			if (distance[curr] != UNKNOWN && distance[e.dst] > distance[curr] + e.weight)
			{
				distance[e.dst] = distance[curr] + e.weight;
			}
		}
	}

	return distance;
}

void Johnson(vector<Edge> edges, int V)
{
	// ���� ������ �߰��� �׷������� �ִ� �Ÿ��� ���
	vector<int> h = BellmanFord(edges, V);

	if (h.empty())
		return;

	// ���� ����ġ �缳��
	for (auto& e : edges)
	{
		e.weight += (h[e.src] - h[e.dst]);
	}

	// ��� ������ ������ �ִ� �Ÿ��� ����
	vector<vector<int>> shortest(V);

	for (int i = 0; i < V; i++)
	{
		shortest[i] = Dijkstra(edges, V, i);
	}

	// ����ġ ��ȯ ������ ������ �����Ͽ� �ִ� �Ÿ��� ���
	for (int i = 0; i < V; i++)
	{
		cout << i << ":\n";

		for (int j = 0; j < V; j++)
		{
			if (shortest[i][j] != UNKNOWN)
			{
				shortest[i][j] += h[j] - h[i];

				cout << "\t" << j << ": " << shortest[i][j] << endl;
			}
		}
	}
}

int main()
{
	int V = 5; // ���� ����
	vector<Edge> edges; // ���� �������� ����

	vector<vector<int>> edge_map{ // {���� ����, ��ǥ ����, ����ġ}
		{0, 1, -7},
		{1, 2, -2},
		{2, 0, 10},
		{0, 3, -5},
		{0, 4, 2},
		{3, 4, 4}
	};

	for (auto& e : edge_map)
	{
		edges.emplace_back(Edge{ e[0], e[1], e[2] });
	}

	Johnson(edges, V);
}

