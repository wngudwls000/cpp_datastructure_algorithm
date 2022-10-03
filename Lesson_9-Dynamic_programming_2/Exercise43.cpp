#include <iostream>
#include <map>
#include <algorithm>
#include <vector>

using namespace std;

const int UNKNOWN = 1e9;

int V, E;
vector<vector<int>> adj;
vector<vector<int>> weight;

map<pair<int, int>, int> memo;

int ShortestPath_Memoization(int depth, int node, vector<vector<int>>& adj, vector<vector<int>>& weight)
{
	// �ʿ� Ű�� �ִ����� Ȯ��
	if (memo.find({ node, depth }) != memo.end())
	{
		return memo[{node, depth}];
	}

	memo[{node, depth}] = UNKNOWN;

	//������ ������ ���� �ݺ�
	for (auto next : adj[node])
	{
		int w = weight[node][next];
		int dist = ShortestPath_Memoization(depth - 1, next, adj, weight) + w;

		memo[{node, depth}] = min(memo[{node, depth}], dist);
	}

	return memo[{node, depth}];
}

vector<int> SingleSourceShortestPaths(int source)
{
	vector<vector<int>> adj_t(V);
	vector<vector<int>> weight_t(V, vector<int>(V, UNKNOWN));

	memo.clear();

	for (int i = 0; i < V; i++)
	{
		// ��ġ �׷��� ����
		for (auto j : adj[i])
		{
			adj_t[j].push_back(i);
			weight_t[j][i] = weight[i][j];
		}

		// ���� ���� - ���� �������� �ڱ� �ڽű����� �ִ� �Ÿ��� �׻� 0

		memo[{source, i}] = 0;

		if (i != source)
		{
			// V-1 �ݺ� �� �ҽ� �̿��� ��忡 ������ ���, ��ΰ� �������� ����
			memo[{i, 0}] = UNKNOWN;
		}
	}

	vector<int> distance(V);

	for (int i = 0; i < V; i++)
	{
		distance[i] = ShortestPath_Memoization(V - 1, i, adj_t, weight_t);
	}

	return distance;
}

int main()
{
	cin >> V >> E;

	weight.resize(V, vector<int>(V, UNKNOWN));
	adj.resize(V);

	for (int i = 0; i < E; i++)
	{
		int u, v, w;
		cin >> u >> v >> w;
		adj[u].push_back(v);
		weight[u][v] = w;
	}

	vector<vector<int>> paths(V);

	for (int i = 0; i < V; i++)
	{
		paths[i] = SingleSourceShortestPaths(i);
	}

	cout << "�� ���� ������ �ִܰŸ�:" << endl;

	for (int i = 0; i < V; i++)
	{
		cout << i << ": ";
		for (int j = 0; j < V; j++)
		{
			(paths[i][j] == UNKNOWN) ? cout << "- " : cout << paths[i][j] << " ";
		}
		cout << endl;
	}
}

/*
 8 20
 0 1 387
 0 3 38
 0 5 471
 1 0 183
 1 4 796
 2 5 715
 3 0 902
 3 1 712
 3 2 154
 3 6 425
 4 3 834
 4 6 214
 5 0 537
 5 3 926
 5 4 125
 5 6 297
 6 1 863
 6 7 248
 7 0 73
 7 3 874
*/