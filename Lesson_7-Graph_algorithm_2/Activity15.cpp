#include <vector>
#include <iostream>
#include <climits>
#include <fstream>

using namespace std;

struct Edge
{
	int src;
	int dst;
	int weight;
};

const int UNKNOWN = INT_MAX;

auto read_file(string filename, int& N, vector<Edge>& edges)
{
	ifstream file(filename);

	if (!file.is_open())
	{
		cout << "�׽�Ʈ ���̽� ������ �� �� �����ϴ�!" << endl;
		return false;
	}
	
	file >> N;

	for (int i = 0; i < N * N - 1; i++)
	{
		int power;
		string direction;
		int next_cell = i;

		file >> direction >> power;

		for (auto& d : direction)
		{
			if (d == 'N')
			{
				next_cell = i - N;
			}
			else if (d == 'E')
			{
				next_cell = i + 1;
			}
			else if (d == 'S')
			{
				next_cell = i + N;
			}
			else
			{
				next_cell = i - 1;
			}
			// power ���� ��ȣ�� �ٲ㼭 ���� ����ġ�� ���
			edges.push_back(Edge{ i, next_cell, -power });

		}

		//for (auto d : direction)
		//{
		//	switch (d)
		//	{
		//	case 'N': next_cell = i - N; break;
		//	case 'E': next_cell = i + 1; break;
		//	case 'S': next_cell = i + N; break;
		//	case 'W': next_cell = i - 1; break;
		//	}

		//	edges.push_back(Edge{ i, next_cell, -power });
		//}
	}

	return true;
}

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
	
	// ���� ����ġ ����Ŭ�� �ִ��� �˻�
	for (auto& e : edges)
	{
		if (distance[e.src] == UNKNOWN)
			continue;
		
		if (distance[e.dst] > distance[e.src] + e.weight)
		{
			return {};
		}
	}
		
	return distance;
}

int main()
{
	int N1;
	int N2;
	int N3;
	int N4;
	int N5;

	vector<Edge> edges1;
	vector<Edge> edges2;
	vector<Edge> edges3;
	vector<Edge> edges4;
	vector<Edge> edges5;

	if (read_file("testcase1.txt", N1, edges1))
	{
		vector<int> distance = BellmanFord(edges1, N1 * N1, 0);

		if (distance.empty() || distance[N1 * N1 - 1] == UNKNOWN)
			cout << "Ž�� �ߴ�" << endl;
		else
			cout << -1 * distance[N1 * N1 - 1] << endl;
	}
	if (read_file("testcase2.txt", N2, edges2))
	{
		vector<int> distance = BellmanFord(edges2, N2 * N2, 0);

		if (distance.empty() || distance[N2 * N2 - 1] == UNKNOWN)
			cout << "Ž�� �ߴ�" << endl;
		else
			cout << -1 * distance[N2 * N2 - 1] << endl;
	}	
	if (read_file("testcase3.txt", N3, edges3))
	{
		vector<int> distance = BellmanFord(edges3, N3 * N3, 0);

		if (distance.empty() || distance[N3 * N3 - 1] == UNKNOWN)
			cout << "Ž�� �ߴ�" << endl;
		else
			cout << -1 * distance[N3 * N3 - 1] << endl;
	}	
	if (read_file("testcase4.txt", N4, edges4))
	{
		vector<int> distance = BellmanFord(edges4, N4 * N4, 0);

		if (distance.empty() || distance[N4 * N4 - 1] == UNKNOWN)
			cout << "Ž�� �ߴ�" << endl;
		else
			cout << -1 * distance[N4 * N4 - 1] << endl;
	}	
	if (read_file("testcase5.txt", N5, edges5))
	{
		vector<int> distance = BellmanFord(edges5, N5 * N5, 0);

		if (distance.empty() || distance[N5 * N5 - 1] == UNKNOWN)
			cout << "Ž�� �ߴ�" << endl;
		else
			cout << -1 * distance[N5 * N5 - 1] << endl;
	}
}

