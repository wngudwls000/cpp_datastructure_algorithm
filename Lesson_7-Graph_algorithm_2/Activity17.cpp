#include <vector>
#include <iostream>
#include <fstream>
#include <climits>
#include <stack>
#include <algorithm>

using namespace std;

struct Edge
{
	int src;
	int dst;
	int weight;
};

const int UNKNOWN = INT_MAX;

bool ReadTestCase(string filename, int& V, vector<Edge>& edges)
{
	ifstream file(filename);;

	if (!file.is_open())
	{
		cout << "테스트 케이스 파일을 열 수 없습니다!" << endl;
		return false;
	}

	int E;
	file >> V >> E;

	for (int i = 0; i < E; i++)
	{
		int u, v, w;
		file >> u >> v >> w;

		edges.push_back(Edge{ u, v, w });
	}

	return true;
}

void FillStack(int node, vector<bool>& visited, 
	vector<vector<int>>& adj, stack<int>& stack)
{
	visited[node] = true;

	for (auto next : adj[node])
	{
		if (!visited[next])
		{
			FillStack(next, visited, adj, stack);
		}
	}

	stack.push(node);
}

vector<bool> isStuck;
vector<int> inComponent;
int componentIndex;

void CollectConnectedComponents(int node, vector<bool>& visited, 
	vector<vector<int>>& adj, vector<int>& component)
{
	visited[node] = true;
	component.push_back(node);

	inComponent[node] = componentIndex;

	for (auto next : adj[node])
	{
		if (!visited[next])
		{
			CollectConnectedComponents(next, visited, adj, component);
		}
		else if (inComponent[node] != inComponent[next])
		{
			isStuck[inComponent[next]] = false;
		}
	}
}

vector<vector<int>> Transpose(int V, vector<vector<int>> adj)
{
	vector<vector<int>> transpose(V);

	for (int i = 0; i < V; i++)
	{
		for (auto next : adj[i])
		{
			transpose[next].push_back(i);
		}
	}

	return transpose;
}

vector<vector<int>> Kosaraju(int V, vector<vector<int>> adj)
{
	isStuck.resize(V, true);
	inComponent.resize(V, UNKNOWN);
	componentIndex = 0;

	vector<bool> visited(V, false);
	stack<int> stack;
	
	for (int i = 0; i < V; i++)
	{
		if (!visited[i])
		{
			FillStack(i, visited, adj, stack);
		}
	}

	vector<vector<int>> transpose = Transpose(V, adj);

	fill(visited.begin(), visited.end(), false);

	vector<vector<int>> connectedComponents;

	while (!stack.empty())
	{
		int node = stack.top();
		stack.pop();

		if (!visited[node])
		{
			vector<int> component;

			CollectConnectedComponents(node, visited, transpose, component);
			connectedComponents.push_back(component);
			componentIndex++;
		}
	}

	return connectedComponents;
}

int BellmanFord(vector<Edge> edges, int V, int start)
{
	vector<int> distance(V, UNKNOWN);
	distance[start] = 0;
	int shortest = UNKNOWN;

	for (int i = 0; i < V - 1; i++)
	{
		for (auto& e : edges)
		{
			if (distance[e.src] == UNKNOWN)
				continue;

			if (distance[e.dst] > distance[e.src] + e.weight)
			{
				distance[e.dst] = distance[e.src] + e.weight;
			}
		}
	}

	for (auto& e : edges)
	{
		if (distance[e.src] == UNKNOWN)
			continue;

		if (distance[e.dst] > distance[e.src] + e.weight)
			return UNKNOWN;
	}

	for (int i = 0; i < V; i++)
	{
		if (i == start) continue;

		shortest = min(shortest, distance[i]);
	}

	return shortest;
}

void test(string filename, int& V, vector<Edge>& edges)
{
	cout << filename << endl;

	vector<vector<int>> adj(V + 1);

	for (auto& e : edges)
	{
		adj[e.src].push_back(e.dst);
	}

	vector<int> results;

	for (int i = 0; i < V; i++)
	{
		if (adj[i].empty())
		{
			results.push_back(UNKNOWN);
			continue;
		}

		int shortest = BellmanFord(edges, V, i);

		if (shortest == UNKNOWN)
		{
			cout << "유효하지 않은 미로" << endl;
			cout << endl;
			return;
		}

		results.push_back(shortest);
	}

	for (int i = 0; i < V; i++)
	{
		if (results[i] == UNKNOWN)
			cout << i << ": 고립된 방" << endl;
		else
			cout << i << ": " << results[i] << endl;
	}

	auto components = Kosaraju(V, adj);
	
	for (int i = 0; i < components.size(); i++)
	{
		if (isStuck[i])
		{
			for (auto node : components[i])
			{
				cout << node << " ";
			}
			cout << endl;
		}
	}

	cout << endl;
}

int main()
{
	int V1;
	int V2;
	int V3;
	int V4;
	int V5;
	int V6;
	int V7;

	vector<Edge> edges1;
	vector<Edge> edges2;
	vector<Edge> edges3;
	vector<Edge> edges4;
	vector<Edge> edges5;
	vector<Edge> edges6;
	vector<Edge> edges7;

	ReadTestCase("miro_testcase1.txt", V1, edges1);
	ReadTestCase("miro_testcase2.txt", V2, edges2);
	ReadTestCase("miro_testcase3.txt", V3, edges3);
	ReadTestCase("miro_testcase4.txt", V4, edges4);
	ReadTestCase("miro_testcase5.txt", V5, edges5);
	ReadTestCase("miro_testcase6.txt", V6, edges6);
	ReadTestCase("miro_testcase7.txt", V7, edges7);

	test("miro_testcase1.txt", V1, edges1);
	test("miro_testcase2.txt", V2, edges2);
	test("miro_testcase3.txt", V3, edges3);
	test("miro_testcase4.txt", V4, edges4);
	test("miro_testcase5.txt", V5, edges5);
	test("miro_testcase6.txt", V6, edges6);
	test("miro_testcase7.txt", V7, edges7);
}