#include <string>
#include <vector>
#include <iostream>
#include <set>
#include <map>
#include <queue>
#include <limits>
#include <algorithm>
#include <fstream>
#include <sstream>

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
			cerr << "에러: 유효 범위를 벗어난 정점!" << endl;
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
auto read_file()
{
	ifstream file("USA-road-d.NY.gr.txt");
	unsigned num_vertices, num_edges;

	string line;

	while (getline(file, line))
	{
		if (line[0] == 'p')
		{
			istringstream iss(line);

			char p;
			string sp;
			iss >> p >> sp >> num_vertices >> num_edges;
			cout << "정점 개수: " << num_vertices << endl;
			cout << "에지 개수: " << num_edges << endl;
			break;
		}
	}

	Graph<T> G(num_vertices + 1);

	while (getline(file, line))
	{
		if (line[0] == 'a')
		{
			istringstream iss(line);
			
			char p;
			unsigned source_vertex, dest_vertex;
			T weight;
			iss >> p >> source_vertex >> dest_vertex >> weight;

			G.add_edge(Edge<T>{source_vertex, dest_vertex, weight});
		}
	}

	file.close(); // 열었던 파일을 닫는다. 
	return G;
}



template <typename T>
struct Label
{
	unsigned ID;
	T distance;

	// Label 객체 비교는 거리(distance) 값을 이용
	inline bool operator> (const Label<T>& l) const
	{
		return this->distance > l.distance;
	}
};

template <typename T>
auto dijkstra_path(const Graph<T>& G, unsigned src, unsigned dst)
{
	// 최소 힙
	priority_queue<Label<T>, vector<Label<T>>, greater<Label<T>>> heap;

	// 모든 정점에서 거리 값을 최대로 설정
	vector<T> distance(G.vertices(), numeric_limits<T>::max());

	set<unsigned> visited; // 방문한 정점
	vector<unsigned> parent(G.vertices()); // 이동 경로 기억을 위한 벡터

	heap.emplace(Label<T>{src, 0});
	parent[src] = src;

	while (!heap.empty())
	{
		auto current_vertex = heap.top();
		heap.pop();

		// 목적지 정점에 도착했다면 종료
		if (current_vertex.ID == dst)
		{
			//cout << current_vertex.ID << "번 정점(목적 정점)에 도착!" << endl;
			break;
		}

		// 현재 정점을 이전에 방문하지 않았다면
		if (visited.find(current_vertex.ID) == visited.end())
		{
			//cout << current_vertex.ID << "번 정점에 안착!" << endl;

			// 현재 정점과 연결된 모든 에지에 대해
			for (auto& e : G.edges(current_vertex.ID))
			{
				auto neighbor = e.dst;
				auto new_distance = current_vertex.distance + e.weight;

				// 인접한 정점의 거리 값이 새로운 경로에 의한 거리 값보다 크면 힙에 추가하고, 거리값을 업데이트함
				if (new_distance < distance[neighbor])
				{
					heap.emplace(Label<T>{neighbor, new_distance});

					parent[neighbor] = current_vertex.ID;
					distance[neighbor] = new_distance;
				}
			}

			visited.insert(current_vertex.ID);
		}
	}

	// 백트래킹 방식으로 시작 정점부터 목적 정점까지의 경로 구성
	vector<unsigned> shortest_path;
	auto current_vertex = dst;

	while (current_vertex != src)
	{
		shortest_path.push_back(current_vertex);
		current_vertex = parent[current_vertex];
	}

	shortest_path.push_back(src);
	reverse(shortest_path.begin(), shortest_path.end());

	return shortest_path;
}

int main()
{
	using T = unsigned;

	auto G = read_file<T>();

	unsigned src_id = 913;
	unsigned dst_id = 542;

	auto shortest_path = dijkstra_path<T>(G, src_id, dst_id);

	cout << endl << "[" << src_id << " 정점에서 " << dst_id << " 정점까지의 최단 경로]" << endl;
	for (auto v : shortest_path)
		cout << v << " ";
	cout << endl;
}